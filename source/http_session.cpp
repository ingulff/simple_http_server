/**
 *  ᛝ
 */

#include <chrono>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <variant>

#include <boost/asio/post.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl/ssl_stream.hpp>

// boost
#include <libs/beast/example/common/root_certificates.hpp>

#include "error.hpp"
#include "http_session.hpp"
#include "utils/algorithm.hpp"

namespace tt_program
{

class http_session::http_session_impl : public std::enable_shared_from_this<http_session_impl>
{
public:
	using ssl = http_session::ssl;
	
	using status = http_session::status;

	using http_method = http_session::http_method;
	
	using connect_t = http_session::connect_t;

	using response_callback_t = http_session::response_callback_t;
public:
	explicit http_session_impl(boost::asio::io_context & io_context, ssl::enabled_ssl)
		: m_resolver(io_context)
		, m_io_context(io_context)
		, m_ssl_context(std::in_place, boost::asio::ssl::context::tlsv13_client)
		, m_stream(std::in_place_type< boost::beast::ssl_stream<boost::beast::tcp_stream> >, io_context, *m_ssl_context)
		, m_buffer()
		, m_request()
		, m_response()
		, m_callback()
		, m_settings()
		, m_status(status::created)
		, m_last_activity(std::chrono::steady_clock::now())
	{
		boost::beast::error_code error;
		// This holds the root certificate used for verification
    	::load_root_certificates(*m_ssl_context, error);
	}

	explicit http_session_impl(boost::asio::io_context & io_context, ssl::disabled_ssl)
		: m_resolver(io_context)
		, m_io_context(io_context)
		, m_ssl_context(std::nullopt)
		, m_stream(std::in_place_type<boost::beast::tcp_stream>, io_context)
		, m_buffer()
		, m_request()
		, m_response()
		, m_callback()
		, m_settings()
		, m_status(status::created)
		, m_last_activity(std::chrono::steady_clock::now())
	{}

	http_session_impl(http_session_impl & other) = delete;

	http_session_impl & operator=(http_session_impl & other) = delete;

	http_session_impl(http_session_impl && other) = delete;

	http_session_impl & operator=(http_session_impl && other) = delete;

	~http_session_impl() = default;

public:

	constexpr status get_status() const noexcept 
	{ 
		return m_status; 
	}


	status set_status(status new_status)
	{ 
		status old = m_status; 
		m_status = new_status; 
		return old; 
	}


	std::chrono::steady_clock::time_point  last_activity() const 
	{ 
		return m_last_activity; 
	}

public:

	void send(connect_t settings, response_callback_t callbask)
	{
		m_callback = std::move(callbask);
		connect_t old_settings = std::exchange(m_settings, std::move(settings) );
		
		if ( (m_status != status::connected && m_status != status::wait_next_request) || old_settings.host != m_settings.host || old_settings.port != m_settings.port)
		{
			
			return connect(m_settings.host, m_settings.port);
		}

		write();
	}

private:

	void connect(std::string host, std::int16_t port)
	{
		m_settings.host = std::move(host);
		m_settings.port = port;

		set_status(status::resolving);

		m_resolver.async_resolve(
			m_settings.host,
			std::to_string(m_settings.port),
			std::bind(
				&http_session_impl::on_resolve,
				this->shared_from_this(), 
				std::placeholders::_1, 
				std::placeholders::_2
			)
		);
	}

	void on_resolve(boost::beast::error_code error, boost::asio::ip::tcp::resolver::results_type results)
	{
		if( error )
		{
			return;
		}

		set_status(status::connecting);

		if (m_ssl_context)
        {
            //set verify mode.
            std::visit(utils::overloaded{ 
                [](boost::beast::tcp_stream&){ assert(false); },
                [this](boost::beast::ssl_stream<boost::beast::tcp_stream>& stream)
                {
//#if 0 //Disable verify, until solve an issue with certificates.
                    stream.set_verify_mode(boost::asio::ssl::verify_peer | boost::asio::ssl::verify_fail_if_no_peer_cert);

                    ::ERR_clear_error();

                    if ( !SSL_set_tlsext_host_name(stream.native_handle(), m_settings.host.c_str()) )
                    {
                        return on_error(boost::asio::error::make_error_code(static_cast<boost::asio::error::ssl_errors>( ::ERR_get_error() )));
                    }
#if 0//#else 
                    stream.set_verify_mode(boost::asio::ssl::verify_none);
#endif
                }
            }, m_stream);    
        }

		auto self_shared = this->shared_from_this();
        std::visit([&results, this, self_shared](auto& stream){
			using namespace std::chrono_literals;
            boost::beast::get_lowest_layer(stream).expires_after(1ms);

            boost::beast::get_lowest_layer(stream).async_connect(results, std::bind(&http_session_impl::on_connect, self_shared, std::placeholders::_1, std::placeholders::_2));

        }, m_stream);

	}

	void on_connect(boost::beast::error_code error, boost::asio::ip::tcp::resolver::results_type::endpoint_type)
	{
		if( error )
		{
			return;
		}
		
		if (m_ssl_context)
        {
			set_status(status::handshaking);

			auto self_shared = this->shared_from_this();
			std::visit(utils::overloaded{
				[](boost::beast::tcp_stream&){ assert(false); },
				[this, self_shared](boost::beast::ssl_stream<boost::beast::tcp_stream>& stream)
				{
					stream.async_handshake(boost::asio::ssl::stream_base::client, std::bind(&http_session_impl::on_handshake, self_shared, std::placeholders::_1));
				}
			}, m_stream);
		}
		else
		{
			std::visit([](auto& stream){ boost::beast::get_lowest_layer(stream).expires_never(); }, m_stream);
            write();
		}
	}

	void on_handshake(const boost::system::error_code&  error)
	{
		if( error )
		{
			return;
		}

		set_status(status::connected);

		std::visit([](auto& stream){ boost::beast::get_lowest_layer(stream).expires_never(); }, m_stream);
        write();
	}

	void write()
	{
		set_status(status::writing);//writing request.
		m_request = {};
        m_response = {};
		m_request.version(m_settings.version);
		m_request.method(m_settings.method == http_method::GET ? boost::beast::http::verb::get : boost::beast::http::verb::post);
        m_request.target(m_settings.target);

		m_request.set(boost::beast::http::field::host, m_settings.host);
        m_request.set(boost::beast::http::field::content_type, "application/tt");
        m_request.set(boost::beast::http::field::content_length, std::to_string(m_settings.target.size()));

		auto self_shared = this->shared_from_this(); //MSVC bug: can't determine this->shared_from_this() inside lambda.
        std::visit([this, self_shared](auto& stream){
            boost::beast::http::async_write(stream, m_request, std::bind(&http_session_impl::on_write, self_shared, std::placeholders::_1, std::placeholders::_2));
        }, m_stream);
	}

	void on_write(boost::beast::error_code error, std::size_t bytes_trasferred)
	{
		if( error )
		{
			return;
		}

		set_status(status::reading);

        auto self_shared = this->shared_from_this();
        std::visit([this, self_shared](auto& stream){
			using namespace std::chrono_literals;
            boost::beast::get_lowest_layer(stream).expires_after(1ms);
            boost::beast::http::async_read(stream, m_buffer, m_response, std::bind(&http_session_impl::on_read, self_shared, std::placeholders::_1, std::placeholders::_2));
        }, m_stream);
	}

	void on_read(boost::beast::error_code error, std::size_t bytes_trasferred)
	{
		if( error )
		{
			return;
		}

		set_status(status::wait_next_request);
        m_last_activity = std::chrono::steady_clock::now();

		if (m_callback)
        {
			auto error_code = m_response.result_int();

			auto callback = std::exchange(m_callback, nullptr);

			auto result = m_response.body().data();

			callback(result, error_code);
		}
	}

	void on_error(const boost::system::error_code& ec) 
	{
        set_status(status::error_occurred);
        m_last_activity = std::chrono::steady_clock::now();
        if (auto callback = std::exchange(m_callback, nullptr))
        {
            auto error = error::to_int(error::errc::cant_send_http);
            //boost::asio::post(m_io_context, std::bind(callback, response, m_current_packet, ec));
            auto result = m_response.body().data();

			callback(result, error);
        }
	}

private:
	using ssl_context_optional = std::optional<boost::asio::ssl::context>;
	using stream_variant_type = std::variant<boost::beast::tcp_stream, boost::beast::ssl_stream<boost::beast::tcp_stream> >;

	using http_response_t	= boost::beast::http::response<boost::beast::http::string_body>;
	using http_request_t    = boost::beast::http::request<boost::beast::http::string_body>;

private:

	boost::asio::ip::tcp::resolver m_resolver;
	boost::asio::io_context & m_io_context;
	ssl_context_optional m_ssl_context;
	stream_variant_type m_stream;
	boost::beast::flat_buffer m_buffer;
	http_request_t m_request;
    http_response_t m_response;
	response_callback_t m_callback;

	connect_t m_settings;

	status  m_status;
	std::chrono::steady_clock::time_point  m_last_activity;
};


http_session::http_session(boost::asio::io_context & io_context, ssl::enabled_ssl)
	: m_impl(  std::make_shared<http_session_impl>(io_context, ssl::enabled_ssl{} )  )
{}

http_session::http_session(boost::asio::io_context & io_context, ssl::disabled_ssl)
	: m_impl( std::make_shared<http_session_impl>(io_context, ssl::disabled_ssl{} ) )
{}

http_session::~http_session() = default;


void http_session::send(connect_t settings, response_callback_t callbask)
{
	m_impl->send(std::move(settings), std::move(callbask) );
}

} // namespace tt_program


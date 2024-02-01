/**
 *  ᛝ
 */


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <filesystem>
#include <system_error>
#include <type_traits>

#include <boost/asio/io_context.hpp>

#include "error/http_error.hpp"
#include "handle_response.hpp"
#include "http_client.hpp"
#include "utils/filepaths.hpp"
#include "utils/filesystem_helpers.hpp"

#include <memory>
#include "http_session.hpp"

//boost::asio::io_context io_context;
//auto settings = tt_tests::make_connect({}, {}, {}, {}, {});

//tt_tests::http_client client( io_context, [](std::string, std::uint32_t) {} );
//tt_tests::upload_filepaths
//tt_tests::download_filepaths


TEST_CASE("prepare_files")
{
	REQUIRE_FALSE(tt_tests::upload_resource_dir.empty());
	REQUIRE_FALSE(tt_tests::download_resourse_dir.empty());

	REQUIRE_NOTHROW(tt_program::utils::throw_if_no_exist(tt_tests::upload_resource_dir));

	REQUIRE_NOTHROW(tt_program::utils::throw_if_no_directory(tt_tests::upload_resource_dir));
	
	REQUIRE_NOTHROW(tt_tests::generate_upload_filepaths(tt_tests::upload_resource_dir));
	REQUIRE_NOTHROW(tt_tests::generate_download_filepaths(tt_tests::download_resourse_dir));

	REQUIRE_FALSE(tt_tests::upload_resource_dir.empty());

	REQUIRE_NOTHROW(tt_tests::generate_upload_file_hashs());
}


TEST_CASE("single_client_session")
{
	boost::asio::io_context io_context;

	tt_tests::http_client client( io_context, { handle_response_body } );
#if 0  // есть некоторый конфликт в сертификатах? boost-дефолтные принимает только boost::server
	SUBCASE("google.com_without_hhtps")
	{
		tt_tests::utils::is_reseived = false;
		tt_tests::utils::is_valid_file = false;
		tt_tests::utils::expected_errc = error::http_errc::moved_permanently;
		auto settings = tt_tests::make_connect("/index.html", "google.com", 443, 11, tt_tests::http_client::http_method::GET);
		client.send_https( settings );

		io_context.run();
		CHECK(tt_tests::utils::is_reseived != false);
	}

	SUBCASE("google.com_invalid_filepath")
	{
		tt_tests::utils::is_reseived = false;
		tt_tests::utils::is_valid_file = false;
		tt_tests::utils::expected_errc = error::http_errc::not_found;
		auto settings = tt_tests::make_connect("/test.txt", "google.com", 443, 11, tt_tests::http_client::http_method::GET);
		client.send_http( settings );

		io_context.run();
		CHECK(tt_tests::utils::is_reseived != false);
	}
#endif
	SUBCASE("single_request_to_http_server_file_no_exist")
	{
		tt_tests::utils::is_reseived = false;
		tt_tests::utils::is_valid_file = false;
		tt_tests::utils::expected_errc = error::http_errc::not_found;
		auto settings = tt_tests::make_connect("/qwerty", "127.0.0.1", 1234, 11, tt_tests::http_client::http_method::GET);

		client.send_https( settings );

		io_context.run();
		CHECK(tt_tests::utils::is_reseived != false);
	}

	SUBCASE("single_request_to_http_server_exist_file")
	{
		tt_tests::utils::is_reseived = false;
		tt_tests::utils::is_valid_file = true;
		tt_tests::utils::expected_errc = error::http_errc::successs;
		auto settings = tt_tests::make_connect("/resource/uploadtest_.txt", "127.0.0.1", 1234, 11, tt_tests::http_client::http_method::GET);

		client.send_https( settings );

		io_context.run();
		CHECK(tt_tests::utils::is_reseived != false);
	}

}


TEST_CASE("clear_section")
{
	REQUIRE_FALSE(tt_tests::download_resourse_dir.empty());

	REQUIRE_NOTHROW(tt_program::utils::throw_if_no_exist(tt_tests::download_resourse_dir));

	REQUIRE_NOTHROW(tt_tests::generate_download_filepaths(tt_tests::download_resourse_dir));

	REQUIRE(tt_tests::clear_downloading_directory() == std::error_code{});
}


#ifndef TT_HANDLE_RESPONSE_POLICY_HPP
#define TT_HANDLE_RESPONSE_POLICY_HPP

#include "error/http_error.hpp"
#include "utils/stream_crc32.hpp"

namespace tt_tests::utils
{

inline error::http_errc expected_errc;


} // tt_tests::utils

void handle_response_body(std::string response, std::uint32_t errc)
{
    std::istringstream stream{std::move(response)};
        
    auto handler = tt_tests::make_stream_crc32();
    auto hash = handler->calc_crc32(stream);

    CHECK(error::http_errc::sucesss == error::to_http_errc(errc));
    CHECK(true == tt_tests::upload_files_hashs.count(hash));
}

void handle_response_error(std::string, std::uint32_t errc)
{
    REQUIRE(tt_tests::utils::expected_errc == error::to_http_errc(errc));
}

#endif // TT_HANDLE_RESPONSE_POLICY_HPP

/**
 *  ᛝ
 */


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <filesystem>

#include <boost/asio/io_context.hpp>

#include "http_client.hpp"
#include "stream_crc32.hpp"
#include "utils/filepaths.hpp"

//boost::asio::io_context io_context;
//auto settings = tt_tests::make_connect({}, {}, {}, {}, {});

//tt_tests::http_client client( io_context, [](std::string, std::uint32_t) {} );
//tt_tests::upload_filepaths
//tt_tests::download_filepaths

TEST_CASE("prepare_files")
{
	REQUIRE_FALSE(tt_tests::upload_resource_dir.empty());
	REQUIRE_FALSE(tt_tests::download_resourse_dir.empty());

	REQUIRE_NOTHROW(tt_tests::throw_if_no_exist(tt_tests::upload_resource_dir));
	REQUIRE_NOTHROW(tt_tests::throw_if_no_exist(tt_tests::download_resourse_dir));

	REQUIRE_NOTHROW(tt_tests::throw_if_no_directory(tt_tests::upload_resource_dir));
	REQUIRE_NOTHROW(tt_tests::throw_if_no_directory(tt_tests::download_resourse_dir));
	
	REQUIRE_NOTHROW(tt_tests::generate_upload_filepaths(tt_tests::upload_resource_dir));
	REQUIRE_NOTHROW(tt_tests::generate_download_filepaths(tt_tests::download_resourse_dir));
}


TEST_CASE("single client session")
{
	boost::asio::io_context io_context;

	auto settings = tt_tests::make_connect({}, {}, {}, {}, {});

	tt_tests::http_client client( io_context, [](std::string, std::uint32_t) {} );
}


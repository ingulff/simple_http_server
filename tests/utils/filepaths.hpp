#ifndef TT_FILEPATHS_HPP
#define TT_FILEPATHS_HPP

#include <string>
#include <vector>
#include <unordered_set>

#include <cstdint>

namespace tt_tests
{

#ifdef UPLOAD_SOURCE_DIR

constexpr std::string_view upload_resource_dir = UPLOAD_SOURCE_DIR;

#else

constexpr std::string_view upload_resource_dir = "./resources/upload";

#endif // UPLOAD_SOURCE_DIR

#ifdef DOWNLOAD_SOURCE_DIR

constexpr std::string_view download_resourse_dir = DOWNLOAD_SOURCE_DIR;

#else

constexpr std::string_view download_resourse_dir = "./resources/download";

#endif // DOWNLOAD_SOURCE_DIR


inline std::vector<std::string> upload_filepaths;

inline std::vector<std::string> download_filepaths;

inline std::unordered_set<std::uint32_t> upload_files_hashs;


void generate_upload_filepaths(const std::string_view & dir_path);
void generate_download_filepaths(const std::string_view & dir_path);

void generate_upload_file_hashs();

} // namespace tt_tests

#endif // TT_FILEPATHS_HPP

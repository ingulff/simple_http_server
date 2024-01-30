/**
 *  ᛝ
 */

#include <stdexcept>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_set>

#include <cstdint>

#include "filepaths.hpp"
#include "stream_crc32.hpp"
#include "utils/filesystem_helpers.hpp"

namespace tt_tests
{

static void append_files(std::vector<std::string> & paths, const std::filesystem::path & path)
{
    for( const auto & elem : std::filesystem::directory_iterator(path) )
    {
        if(std::filesystem::is_regular_file(elem))
        {
            paths.emplace_back(elem.path().string());
            continue;
        }

        append_files(paths, elem);
    }
}

static void append_files(std::vector<std::string> & paths, const std::string_view & resource_dirs)
{
    std::filesystem::path directory{resource_dirs};

    append_files(paths, directory);
}


static std::vector<std::string> filenames;

void generate_upload_filepaths(const std::string_view & dir_path)
{
    filenames.clear();

    upload_filepaths.clear();
    
    std::filesystem::path entry{dir_path};

    tt_program::utils::throw_if_no_exist(entry);
    tt_program::utils::throw_if_no_directory(entry);

    append_files(upload_filepaths, entry);
}

void generate_download_filepaths(const std::string_view & dir_path)
{
    download_filepaths.clear();
    
    if(filenames.empty() == true)
    {
        generate_upload_filepaths(upload_resource_dir);
    }

    std::filesystem::path entry{dir_path};

    tt_program::utils::throw_if_no_exist(entry);
    tt_program::utils::throw_if_no_directory(entry);

    append_files(download_filepaths, entry);
}


static void insert_fil_hash(std::unordered_set<std::uint32_t> hash_set, std::int32_t hash)
{
    hash_set.insert(hash);
}


static void generate_file_hash(const std::filesystem::path entry)
{
    tt_program::utils::throw_if_no_exist(entry);
    tt_program::utils::throw_if_no_regular_file(entry);

    auto hasher = make_stream_crc32();
    auto hash = hasher->calc_crc32(entry.string());
    insert_fil_hash(upload_files_hashs, hash);
}

static void generate_file_hash(const std::string & file_path)
{
    std::filesystem::path file{file_path};
    generate_file_hash(file);
}

void generate_upload_file_hashs()
{
    if(upload_filepaths.empty() == true)
    {
        generate_upload_filepaths(upload_resource_dir);
    }

    for(const auto & path : upload_filepaths )
    {
        generate_file_hash(path);
    }
}

} // namespace tt_tests


/**
 *  ᛝ
 */

#include <stdexcept>
#include <filesystem>
#include <string>
#include <vector>

#include "filepaths.hpp"

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


void throw_if_no_exist(const std::filesystem::path & entry)
{
    if(std::filesystem::exists(entry) == false)
    {
        std::string msg{"ERROR: file or directory "};
        msg += entry.string();
        msg += " not exist\n";
        throw std::logic_error(msg);
    }
}

void throw_if_no_exist(const std::string & path)
{
    std::filesystem::path entry{path};

    throw_if_no_exist(entry);
}


void throw_if_no_directory(const std::filesystem::path & dir_path)
{
    if(std::filesystem::is_directory(upload_resource_dir) == false)
    {
        std::string msg{"ERROR: source_dir path: "};
        msg += upload_resource_dir;
        msg += " not directory\n";
        throw std::logic_error(msg);
    }
}

void throw_if_no_directory(const std::string & dir_path)
{
    std::filesystem::path entry{dir_path};

    throw_if_no_directory(entry);
}


static std::vector<std::string> filenames;

void generate_upload_filepaths(const std::string_view & dir_path)
{
    filenames.clear();

    upload_filepaths.clear();
    
    std::filesystem::path entry{dir_path};

    throw_if_no_exist(entry);
    throw_if_no_directory(entry);

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

    throw_if_no_exist(entry);
    throw_if_no_directory(entry);

    append_files(download_filepaths, entry);
}

} // namespace tt_tests


#pragma once

#include <filesystem>
#include <string>
#include <cstdint>

struct FileEntry {
    std::filesystem::path file_path;
    std::string name;
    std::string extension;
    std::uintmax_t size;
};

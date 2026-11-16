#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <cstdint>

struct FileEntry {
    std::filesystem::path file_path;
    std::string name;
    std::string extension;
    std::uintmax_t size;
};

inline constexpr std::string_view LINUX_SAVE_PATH = ".local/share/Nurv/";
inline constexpr std::string_view MACOS_SAVE_PATH = "Library/Application Support/Nurv/";

#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <cstdint>
#include <nlohmann/json.hpp>

struct FileEntry {
    std::filesystem::path file_path;
    std::string name;
    std::string extension;
    std::uintmax_t size;
};

void to_json(nlohmann::json& json, const FileEntry& file);
void from_json(const nlohmann::json& json, FileEntry& file);

inline constexpr std::string      SAVE_FILE_NAME  = "index.json";
inline constexpr std::string_view LINUX_SAVE_PATH = ".local/share/Nurv/";
inline constexpr std::string_view MACOS_SAVE_PATH = "Library/Application Support/Nurv/";

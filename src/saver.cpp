#include "saver.hpp"
#include "types.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <nlohmann/json.hpp>

namespace {
    std::filesystem::path detectIndexPath() {
        const char* home_dir = std::getenv("HOME");
        if (home_dir == nullptr) {
            throw std::runtime_error("HOME environment variable not found");
        }
        #if defined (__APPLE__) && defined (__MACH__)
            return std::filesystem::path(home_dir) / MACOS_SAVE_PATH;
        #else 
            return std::filesystem::path(home_dir) / LINUX_SAVE_PATH;
        #endif
    }

    void createDir(const std::filesystem::path saveDir) {
        std::error_code ec;
        std::filesystem::create_directories(saveDir, ec);
        if (ec) {
            throw std::runtime_error("Failed to create directory: " + saveDir.string());
        }
        std::cout << "Dir was created\n";
    }

    bool dirIsCreated() {
        std::filesystem::path saveDir = detectIndexPath();
        if (std::filesystem::is_directory(saveDir)) {
            return true;
        } else {
            return false;
        }
    }

    nlohmann::json getLoadedJson(const std::vector<FileEntry>& filesInfo) {
        nlohmann::json jsonContent;
        //jsonContent["scanned_at"] = getCurrentStringDateTime();
        jsonContent["files_found"] = filesInfo.size();
        jsonContent["files"] = filesInfo;
        return jsonContent;
    }

    nlohmann::json loadJsonFromFile(const std::filesystem::path& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + path.string());
        }
        nlohmann::json data;
        file >> data;
        return data;
    }

}

void to_json(nlohmann::json& json, const FileEntry& file) {
    json = {{"path", file.file_path.string()}, {"name", file.name}, {"extension", file.extension}, {"size", file.size}};
}

void from_json(const nlohmann::json& json, FileEntry& file) {
    file.file_path = json.at("path").get<std::string>();
    file.name = json.at("name").get<std::string>();
    file.extension = json.at("extension").get<std::string>();
    file.size = json.at("size").get<std::uintmax_t>();
}

void saveToJson(const std::vector<FileEntry>& files_info) {
    std::filesystem::path path_to_save = detectIndexPath();
    if (!dirIsCreated()) {
        createDir(path_to_save);
    }
    nlohmann::json loadedJson = getLoadedJson(files_info);
    std::filesystem::path file_path = path_to_save / SAVE_FILE_NAME;
    std::ofstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path_to_save.string());
    }
    file << loadedJson.dump(4);
    std::cout << "All information about the files was saved to the file " << file_path.string();
}

std::vector<FileEntry> loadFromJson() {
    std::filesystem::path path_from_load = detectIndexPath();
    if (!dirIsCreated()) {
        throw std::runtime_error("There is no Nurv index directory");
    }
    std::filesystem::path file_path = path_from_load / SAVE_FILE_NAME;
    nlohmann::json loadedFiles = loadJsonFromFile(file_path);
    return loadedFiles.at("files").get<std::vector<FileEntry>>();
}

#include "saver.hpp"
#include "types.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <nlohmann/json.hpp>

namespace {
    std::filesystem::path detectPathToSave() {
        const char* home_dir = std::getenv("HOME");
        if (home_dir == nullptr) {
            throw std::runtime_error("HOME environment variable not found");
        }
        #if defined (__APPLE__) && (__MACH__)
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

    bool saveToDirIsCreated() {
        std::filesystem::path saveDir = detectPathToSave();
        if (std::filesystem::is_directory(saveDir)) {
            return true;
        } else {
            return false;
        }
    }

    nlohmann::json fileEntryToJson(const FileEntry& file) {
        return { {"path", file.file_path.string()}, {"name", file.name}, 
                 {"extension", file.extension}, {"size", file.size}};
    }

    nlohmann::json loadJson(const std::vector<FileEntry>& filesInfo) {
        nlohmann::json jsonContent;
        //jsonContent["scanned_at"] = getCurrentStringDateTime();
        jsonContent["files_found"] = filesInfo.size();
        jsonContent["files"] = nlohmann::json::array();
        for (const auto& file : filesInfo) {
            jsonContent["files"].push_back(fileEntryToJson(file));
        }
        return jsonContent;
    }

}

void saveToJson(const std::vector<FileEntry>& files_info) {
    std::filesystem::path path_to_save = detectPathToSave();
    if (!saveToDirIsCreated()) {
        createDir(path_to_save);
    }
    nlohmann::json loadedJson = loadJson(files_info);
    std::filesystem::path file_path = path_to_save / "index.json";
    std::ofstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path_to_save.string());
    }
    file << loadedJson.dump(4);

}

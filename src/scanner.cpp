#include "scanner.hpp"

#include <deque>
#include <filesystem>

namespace {
    FileEntry getFileEntry(const std::filesystem::directory_entry& entry) {
        FileEntry file;
        file.file_path = entry.path();
        file.name = entry.path().filename().string();
        file.extension = entry.path().extension().string();
        file.size = entry.file_size();
        return file;
    }

    std::vector<FileEntry> walkthroughDir(const std::string& targetDir) {
        std::vector<FileEntry> walked_files;
        std::deque<std::filesystem::path> directories_left;
        if (!std::filesystem::is_directory(targetDir)) {
            throw std::invalid_argument("The target path " + targetDir + " is not a directory");
        }
        directories_left.push_back(targetDir);
        while (!directories_left.empty()) {
            std::filesystem::path current = directories_left.front();
            directories_left.pop_front();
            for (const auto& entry : std::filesystem::directory_iterator(targetDir)) {
                if (entry.is_directory()) {
                    directories_left.push_back(entry.path());
                } else if (entry.is_regular_file()) {
                    FileEntry file = getFileEntry(entry);
                    walked_files.push_back(file);
                }
            }
        }
        return walked_files;
    }
    template <typename T>
    void addToVec(T& dest, const T& source) {
        for (const auto& src : source) {
            dest.push_back(src);
        }
    }
}

std::vector<FileEntry> scan(const std::vector<std::string>& targetDirectories) {
    std::vector<FileEntry> final_files;
    for (const auto& target : targetDirectories) {
        std::vector<FileEntry> walked = walkthroughDir(target);
        addToVec(final_files, walked);
    }
    return final_files;
}

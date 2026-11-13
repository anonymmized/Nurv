#include "scanner.hpp"

#include <deque>
#include <filesystem>

std::vector<FileEntry> scan(const std::vector<std::string>& targetDirectories) {
    std::vector<FileEntry> final_files;
    for (const auto& target : targetDirectories) {
        std::deque<std::filesystem::path> directories_left;
        if (!std::filesystem::is_directory(target)) {
            throw std::invalid_argument("The target path " + target + " is not a directory");
        }
        directories_left.push_back(target);
        while (!directories_left.empty()) {
            std::filesystem::path current = directories_left.front();
            directories_left.pop_front();
           for (const auto& entry : std::filesystem::directory_iterator(target)) {
                if (entry.is_directory()) {
                    directories_left.push_back(entry.path());
                } else if (entry.is_regular_file()){
                    FileEntry file;
                    file.file_path = entry.path();
                    file.name = entry.path().filename().string();
                    file.extension = entry.path().extension().string();
                    file.size = entry.file_size();
                    final_files.push_back(file);
                }
           }
        }
    }
    return final_files;
}

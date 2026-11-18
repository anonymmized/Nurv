#include "search.hpp"
#include "types.hpp"

#include <vector>
#include <string>

std::vector<FileEntry> searchByName(const std::vector<FileEntry>& files, const std::string& targetName) {
    if (files.empty()) {
        throw std::runtime_error("Scanned directory is empty");
    }
    std::vector<FileEntry> finalFiles;
    for (const auto& file : files) {
        if (file.name == targetName) {
            finalFiles.push_back(file);
        }
    }
    return finalFiles;
}

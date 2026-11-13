#include <iostream>
#include <filesystem>
#include <vector>
#include <deque>

int main() {
    std::filesystem::path fp = "/Users/insany/Documents/osnova/proga/cpp/Study/CPP Foundation/Nurv";
    std::deque<std::filesystem::path> directories;
    std::vector<std::filesystem::path> files;
    std::vector<std::filesystem::path> foundDirs;
    directories.push_back(fp);
    while (!directories.empty()) {
        for (const auto& dirEntry : std::filesystem::directory_iterator(directories[0])) {
            if (dirEntry.is_directory()) {
                directories.push_back(dirEntry.path());
                foundDirs.push_back(dirEntry.path());
            } else {
                files.push_back(dirEntry.path());
            }
        }
        directories.pop_front();
    }
    std::cout << "Directories found: " << foundDirs.size() << '\n';
    std::cout << "Files found: " << files.size() << '\n';
    for (const auto& directory : foundDirs) {
        std::cout << "Dir: " << directory << '\n';
    }
    for (const auto& file : files) {
        std::cout << "File: " << file << '\n';
    }
    return 0;
}

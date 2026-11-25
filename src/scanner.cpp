#include "scanner.hpp"

#include <deque>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <filesystem>

namespace {
    FileEntry getFileEntry(const std::filesystem::directory_entry& entry) {
        FileEntry file;
        file.file_path = std::filesystem::absolute(entry.path()).lexically_normal();
        file.name = entry.path().filename().string();
        file.extension = entry.path().extension().string();
        file.size = entry.file_size();
        return file;
    }

    ScanResult walkthroughDir(const std::string& targetDir) {
        if (!std::filesystem::is_directory(targetDir)) {
            throw std::invalid_argument("The target path " + targetDir + " is not a directory");
        }
        ScanResult scanResult;
        std::filesystem::path fs_dir = targetDir;
        for (const auto& entry : std::filesystem::directory_iterator(fs_dir)) {
            if (entry.is_directory()) {
                scanResult.found_dirs.push_back(entry.path());
            } else if (entry.is_regular_file()) {
                FileEntry file = getFileEntry(entry);
                scanResult.found.push_back(file);
            }
        }
        return scanResult;
    }
}

void scan_once(std::queue<std::filesystem::path>& tasks, std::vector<FileEntry>& final_vec, std::mutex& queue_mtx, std::mutex& final_mtx) {
    while (true) {
        std::string targetDir;
        {
            std::lock_guard<std::mutex> lock(queue_mtx);
            if (tasks.empty()) {
                break;
            }
            targetDir = tasks.front();
            tasks.pop();
        }
        auto scanResult = walkthroughDir(targetDir);
        {
            std::lock_guard<std::mutex> lock(final_mtx);
            final_vec.insert(final_vec.end(), scanResult.found.begin(), scanResult.found.end());
        }

        {
            std::lock_guard<std::mutex> lock(queue_mtx);
            for (const auto& dir : scanResult.found_dirs) {
                tasks.push(dir);
            }
        }
    }
}

std::vector<FileEntry> scan(const std::vector<std::string>& targetDirectories) {
    std::vector<FileEntry> final_vec;
    std::queue<std::filesystem::path> tasks;
    std::vector<std::thread> threads;
    std::size_t threadCount = std::thread::hardware_concurrency();
    if (threadCount == 0) {
        threadCount = 4;
    }
    std::mutex queue_mtx;
    std::mutex final_mtx;
    for (const auto& targetDir : targetDirectories) {
        tasks.push(targetDir);
    }
    for (size_t i = 0; i < threadCount; i++) {
        threads.emplace_back(scan_once, std::ref(tasks), std::ref(final_vec), std::ref(queue_mtx), std::ref(final_mtx));
    }
    for (auto& thread : threads) {
        thread.join();
    }
    return final_vec;
}

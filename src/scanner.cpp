#include "scanner.hpp"

#include <deque>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <filesystem>

namespace {
    std::optional<FileEntry> getFileEntry(const std::filesystem::directory_entry& entry, std::error_code& ec) {
        FileEntry file;
        file.file_path = std::filesystem::absolute(entry.path(), ec).lexically_normal();
        if (ec) {
            return std::nullopt;
        }
        file.name = entry.path().filename().string();
        file.extension = entry.path().extension().string();
        file.size = entry.file_size(ec);
        if (ec) {
            return std::nullopt;
        }
        return file;
    }

    ScanResult walkthroughDir(const std::string& targetDir) {
        if (!std::filesystem::is_directory(targetDir)) {
            throw std::invalid_argument("The target path " + targetDir + " is not a directory");
        }
        ScanResult scanResult;
        std::filesystem::path fs_dir = targetDir;
        std::error_code out_ec;
        std::filesystem::directory_iterator it(fs_dir, out_ec);
        std::filesystem::directory_iterator end;
        while (it != end) {
            const auto& entry = *it;
            std::error_code in_ec;
            if (entry.is_directory()) {
                scanResult.found_dirs.push_back(entry.path());
            } else if {
                auto file = getFileEntry(entry, in_ec);
                if (file.has_value()) {
                    scanResult.found.push_back(file.value());
                } else {
                    scanResult.errors.push_back({entry.path(), in_ec});
                }
            }
            it.increment(out_ec);
            if (out_ec) {
                scanResult.errors.push_back({entry.path(), out_ec});
            }
        }
        return scanResult;
    }
}

void scan_once(NeedsForScan& data) {
    while (true) {
        std::string targetDir;
        {
            std::unique_lock<std::mutex> lock(data.queue_mtx);
            data.cv.wait(lock, [&] { return !data.tasks.empty() || data.activeWorkers == 0; });
            if (data.tasks.empty()) {
                break;
            }
            targetDir = data.tasks.front();
            data.tasks.pop();
            ++data.activeWorkers;
        }
        auto scanResult = walkthroughDir(targetDir);
        {
            std::lock_guard<std::mutex> lock(data.errors_mtx);
            data.errors.insert(data.errors.end(), scanResult.begin(), scanResult.end());
        }
        {
            std::lock_guard<std::mutex> lock(data.final_mtx);
            data.final_vec.insert(data.final_vec.end(), scanResult.found.begin(), scanResult.found.end());
        }

        {
            std::lock_guard<std::mutex> lock(data.queue_mtx);
            for (const auto& dir : scanResult.found_dirs) {
                data.tasks.push(dir);
            }
            --data.activeWorkers;
        }
        data.cv.notify_all();
    }
}

std::vector<FileEntry> scan(const std::vector<std::string>& targetDirectories) {
    NeedsForScan nfs;
    std::vector<std::thread> threads;
    std::size_t threadCount = std::thread::hardware_concurrency();
    if (threadCount == 0) {
        threadCount = 4;
    }
    for (const auto& targetDir : targetDirectories) {
        nfs.tasks.push(targetDir);
    }
    for (size_t i = 0; i < threadCount; i++) {
        threads.emplace_back(scan_once, std::ref(nfs));
    }
    for (auto& thread : threads) {
        thread.join();
    }
    for (const auto& error : nfs.errors) {
        std::cerr << "[ERROR] " << error.path() << ": " << error.error.message() << '\n';
    }
    return std::move(nfs.final_vec);
}

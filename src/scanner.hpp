#pragma once

#include "parse_cmd_line.hpp"
#include "types.hpp"

#include <vector>
#include <string>
#include <system_error>
#include <optional>

struct ScanError {
    std::filesystem::path error_path;
    std::error_code error;
};

struct ScanResult {
    std::vector<FileEntry> found;
    std::vector<ScanError> errors;
    std::vector<std::filesystem::path> found_dirs;
};

struct NeedsForScan {
    std::vector<FileEntry> final_vec;
    std::queue<std::filesystem::path> tasks;
    std::vector<ScanError> errors;
    std::mutex final_mtx;
    std::mutex queue_mtx;
    std::mutex errors_mtx;
    std::condition_variable cv;
    size_t activeWorkers = 0;

    std::exception_ptr fatalError = nullptr;
    bool stop = false;
};

std::vector<FileEntry> scan(const std::vector<std::string>& targetDirectories);

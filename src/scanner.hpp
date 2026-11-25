#pragma once

#include "parse_cmd_line.hpp"
#include "types.hpp"

#include <vector>
#include <string>

struct ScanResult {
    std::vector<FileEntry> found;
    std::vector<std::filesystem::path> found_dirs;
};

std::vector<FileEntry> scan(const std::vector<std::string>& targetDirectories);

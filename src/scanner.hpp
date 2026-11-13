#pragma once

#include "parse_cmd_line.hpp"
#include "types.hpp"

#include <vector>
#include <string>

std::vector<FileEntry> scan(const std::vector<std::string>& targetDirectories);

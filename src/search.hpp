#pragma once

#include "types.hpp"

#include <string>
#include <vector>

std::vector<FileEntry> searchByName(const std::vector<FileEntry>& files, const std::string& targetName);

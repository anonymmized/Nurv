#pragma once

#include "types.hpp"

#include <string>
#include <vector>

std::vector<FileEntry> searchByName(const std::vector<FileEntry>& files, const std::string& targetName);
std::vector<FileEntry> searchByExtension(const std::vector<FileEntry>& files, const std::string& targetExtension);
std::vector<FileEntry> searchBySize(const std::vector<FileEntry>& files, const std::string& targetSize);

#include "search.hpp"
#include "types.hpp"

#include <vector>
#include <string>
#include <cstdint>

namespace {
    enum class ComparisonType {
        Less,
        LessOrEqual,
        Equal,
        GreaterOrEqual,
        Greater
    };

    struct ComparisonOperator {
        ComparisonType type;
        std::uintmax_t value;
    };

    bool onlyDigits(const std::string& s) {
        if (s.empty()) {
            return false;
        }
        for (char c : s) {
            if (c < '0' || c > '9') {
                return false;
            }
        }
        return true;
    }

    std::uintmax_t getOnlyDigits(const std::string& in_str, size_t first_position) {
        if (in_str.size() <= first_position) {
            throw std::invalid_argument("Number is missing");
        }
        std::string only_digits = in_str.substr(first_position);
        if (!onlyDigits(only_digits)) {
            throw std::invalid_argument("Usage: Operator + Number: >1000 or =500");
        }
        return std::stoull(only_digits);
    }

    ComparisonOperator getOperator(const std::string& in_str) {
        ComparisonOperator cp_op;
        if (in_str.size() < 2) {
            throw std::invalid_argument("Usage: >100, >=100, <100, <=100 or =100");
        }
        if (in_str[0] == '>' && in_str[1] == '=') {
            if (in_str.size() < 3) {
                throw std::invalid_argument("Number is missing after >=");
            }
            cp_op.type = ComparisonType::GreaterOrEqual;
            cp_op.value = getOnlyDigits(in_str, 2);
        } else if (in_str[0] == '<' && in_str[1] == '=') {
            if (in_str.size() < 3) {
                throw std::invalid_argument("Number is missing after <=");
            }
            cp_op.type = ComparisonType::LessOrEqual;
            cp_op.value = getOnlyDigits(in_str, 2);
        } else if (in_str[0] == '>') {
            cp_op.type = ComparisonType::Greater;
            cp_op.value = getOnlyDigits(in_str, 1);
        } else if (in_str[0] == '<') {
            cp_op.type = ComparisonType::Less;
            cp_op.value = getOnlyDigits(in_str, 1);
        } else if (in_str[0] == '=') {
            cp_op.type = ComparisonType::Equal;
            cp_op.value = getOnlyDigits(in_str, 1);
        } else {
            throw std::invalid_argument("Unknown comparison operator");
        }
        return cp_op;
    }

};

std::vector<FileEntry> searchByName(const std::vector<FileEntry>& files, const std::string& targetName) {
    if (files.empty()) {
        throw std::runtime_error("Scanned directory is empty");
    }
    std::vector<FileEntry> finalFiles;
    for (const auto& file : files) {
        size_t position = file.name.find(targetName);
        if (position != std::string::npos) {
            finalFiles.push_back(file);
        }
    }
    return finalFiles;
}

std::vector<FileEntry> searchByExtension(const std::vector<FileEntry>& files, const std::string& targetExtension) {
    if (files.empty()) {
        throw std::runtime_error("Scanned directory is empty");
    }
    std::string extension = targetExtension;
    if (extension.empty()) {
        throw std::invalid_argument("Extension cannot be empty");
    }
    if (extension[0] != '.') {
        extension = "." + extension;
    }
    std::vector<FileEntry> finalFiles;
    for (const auto& file : files) {
        if (file.extension == extension) {
            finalFiles.push_back(file);
        }
    }
    return finalFiles;
}

std::vector<FileEntry> searchBySize(const std::vector<FileEntry>& files, const std::string& targetSize) {
    if (files.empty()) {
        throw std::runtime_error("Scanned directory is empty");
    }
    ComparisonOperator comp_op = getOperator(targetSize);
    std::vector<FileEntry> finalFiles;
    for (const auto& file : files) {
        bool matches = false;
        switch (comp_op.type) {
            case ComparisonType::Less:
                matches = file.size < comp_op.value;
                break;
            case ComparisonType::LessOrEqual:
                matches = file.size <= comp_op.value;
                break;
            case ComparisonType::Equal:
                matches = file.size == comp_op.value;
                break;
            case ComparisonType::GreaterOrEqual:
                matches = file.size >= comp_op.value;
                break;
            case ComparisonType::Greater:
                matches = file.size > comp_op.value;
                break;
        }
        if (matches) {
            finalFiles.push_back(file);
        }
    }
    return finalFiles;
}

std::vector<FileEntry> searchByPath(const std::vector<FileEntry>& files, const std::string& targetPath) {
    if (files.empty()) {
        throw std::runtime_error("Scanned directory is empty");
    }
    std::vector<FileEntry> finalFiles;
    for (const auto& file : files) {
        if (file.file_path.string().find(targetPath) != std::string::npos) {
            finalFiles.push_back(file);
        }
    }
    return finalFiles;
}

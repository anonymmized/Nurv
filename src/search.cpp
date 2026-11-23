#include "search.hpp"
#include "types.hpp"

#include <vector>
#include <string>

namespace {
    struct ComparisonOperator {
        int less = 0;
        int lessOrEqual = 0;
        int equal = 0;
        int greaterOrEqual = 0;
        int greater = 0;
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

    int getOnlyDigits(const std::string& in_str, int first_position) {
        std::string only_digits = in_str.substr(first_position);
        if (!onlyDigits(only_digits)) {
            throw std::invalid_argument("Usage: Operator + Number: >1000 or =500");
        }
        return std::stoi(only_digits);
    }

    ComparisonOperator getOperator(const std::string& in_str) {
        ComparisonOperator cp_op;
        if (in_str[0] == '>' && (in_str[1] >= '0' && in_str[1] <= '9')) {
            cp_op.greater = getOnlyDigits(in_str, 1); 
        } else if (in_str[0] == '<' && (in_str[1] >= '0' && in_str[1] <= '9')) {
            cp_op.less = getOnlyDigits(in_str, 1);
        } else if (in_str[0] == '=' && (in_str[1] >= '0' && in_str[1] <= '9')) {
            cp_op.equal = getOnlyDigits(in_str, 1);
        } else if (in_str[0] == '>' && in_str[1] == '=' && (in_str[2] >= '0' && in_str[2] <= '9')) {
            cp_op.greaterOrEqual = getOnlyDigits(in_str, 2);
        } else if (in_str[0] == '<' && in_str[1] == '=' && (in_str[2] >= '0' && in_str[2] <= '9')) {
            cp_op.lessOrEqual = getOnlyDigits(in_str, 2);
        } else {
            throw std::invalid_argument("There is no operator like this");
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
        if (file.name == targetName) {
            finalFiles.push_back(file);
        }
    }
    return finalFiles;
}

std::vector<FileEntry> searchByExtension(const std::vector<FileEntry>& files, const std::string& targetExtension) {
    if (files.empty()) {
        throw std::runtime_error("Scanned directory is empty");
    }
    std::vector<FileEntry> finalFiles;
    for (const auto& file : files) {
        if (file.extension == targetExtension) {
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
        if (comp_op.less > 0 && file.size < comp_op.less) {
            finalFiles.push_back(file);
        } 
        if (comp_op.lessOrEqual > 0 && file.size <= comp_op.lessOrEqual) {
            finalFiles.push_back(file);
        }
        if (comp_op.equal > 0 && file.size == comp_op.equal) {
            finalFiles.push_back(file);
        }
        if (comp_op.greaterOrEqual > 0 && file.size >= comp_op.greaterOrEqual) {
            finalFiles.push_back(file);
        }
        if (comp_op.greater > 0 && file.size > comp_op.greater) {
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

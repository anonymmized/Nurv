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
        if (in_str.size() <= first_position) {
            throw std::invalid_argument("Number is missing after operator");
        }
        std::string only_digits = in_str.substr(first_position);
        if (!onlyDigits(only_digits)) {
            throw std::invalid_argument("Usage: Operator + Number: >1000 or =500");
        }
        return std::stoi(only_digits);
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
            cp_op.greaterOrEqual = getOnlyDigits(in_str, 2);
        } else if (in_str[0] == '<' && in_str[1] == '=') {
            if (in_str.size() < 3) {
                throw std::invalid_argument("Number is missing after <=");
            }
            cp_op.lessOrEqual = getOnlyDigits(in_str, 2);
        } else if (in_str[0] == '>') {
            cp_op.greater = getOnlyDigits(in_str, 1);
        } else if (in_str[0] == '<') {
            cp_op.less = getOnlyDigits(in_str, 1);
        } else if (in_str[0] == '=') {
            cp_op.equal = getOnlyDigits(in_str, 1);
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

#include "app.hpp"
#include "scanner.hpp"
#include "types.hpp"
#include "saver.hpp"
#include "search.hpp"

#include <vector>
#include <iostream>

namespace {
    template <typename SearchFunction>
    void processSearch(const std::vector<FileEntry>& files, const std::vector<std::string>& targets, SearchFunction searchFunction) {
        for (const auto& target : targets) {
            std::vector<FileEntry> found = searchFunction(files, target);
            if (found.empty()) {
                std::cout << "Nothing found for: " << target << '\n';
                continue;
            }
            std::cout << "Found for \"" << target << "\":\n";
            for (const auto& file : found) {
                std::cout << '\t' << file.file_path << '\n';
            }
            std::cout << '\n';
        }
    }
}

void App::processAllSearches(const std::vector<FileEntry>& loaded) {
    processSearch(loaded, cmd.names_to_find, searchByName);
    processSearch(loaded, cmd.extensions_to_find, searchByExtension);
    processSearch(loaded, cmd.path_to_find, searchByPath);
    processSearch(loaded, cmd.size_to_find, searchBySize);
}

void App::run() {
    if (cmd.argument == Arguments::Scan) {
        std::vector<FileEntry> scanned = scan(cmd.targetDirectory);
        if (scanned.empty()) {
            throw std::runtime_error("There is no files in directory");
        }
        saveToJson(scanned);
    } else if (cmd.argument == Arguments::Search) {
        std::vector<FileEntry> loaded = loadFromJson();
        if (loaded.empty()) {
            throw std::runtime_error("There are no scanning sessions before");
        }
        if (cmd.names_to_find.empty() && cmd.extensions_to_find.empty() && cmd.path_to_find.empty() && cmd.size_to_find.empty()) {
            throw std::invalid_argument("At least one filtering arguments must be selected");
        }
        processAllSearches(loaded);
    }
}

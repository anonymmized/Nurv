#include "app.hpp"
#include "scanner.hpp"
#include "types.hpp"
#include "saver.hpp"
#include "search.hpp"

#include <vector>
#include <iostream>

namespace {
    template <typename SearchFunction>
    std::vector<FileEntry> processSearch(const std::vector<FileEntry>& files, const std::vector<std::string>& targets, SearchFunction searchFunction) {
        std::vector<FileEntry> finalFound;
        for (const auto& target : targets) {
            std::vector<FileEntry> found = searchFunction(files, target);
            finalFound.insert(finalFound.end(), found.begin(), found.end());
        }
        return finalFound;
    }
    
    void printMatches(const std::vector<FileEntry>& matches) {
        std::cout << "Found " << matches.size() << " matches\n";
        for (const auto& match : matches) {
            std::cout << '\t' << match.file_path << '\n';
        }
    }
}

void App::processAllSearches(const std::vector<FileEntry>& loaded) {
    std::vector<FileEntry> allFoundMatches = loaded;
    if (!cmd.names_to_find.empty()) {
        allFoundMatches = processSearch(allFoundMatches, cmd.names_to_find, searchByName);
    }
    if (!cmd.extensions_to_find.empty()) {
        allFoundMatches = processSearch(allFoundMatches, cmd.extensions_to_find, searchByExtension);
    }
    if (!cmd.path_to_find.empty()) {
        allFoundMatches = processSearch(allFoundMatches, cmd.path_to_find, searchByPath);
    }
    if (!cmd.size_to_find.empty()) { 
        allFoundMatches = processSearch(allFoundMatches, cmd.size_to_find, searchBySize);
    }
    printMatches(allFoundMatches);
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

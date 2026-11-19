#include "app.hpp"
#include "scanner.hpp"
#include "types.hpp"
#include "saver.hpp"
#include "search.hpp"

#include <vector>
#include <iostream>

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
            throw std::runtime_error("There is no scanning sessions before");
        }
        if (cmd.names_to_find.empty() && cmd.extensions_to_find.empty() && cmd.path_to_find.empty() && cmd.size_to_find.empty()) {
            throw std::invalid_argument("At least one filtering argument must be selected");
        }
        for (const auto& filename : cmd.names_to_find) {
            std::vector<FileEntry> found_vector = searchByName(loaded, filename);
            if (!found_vector.empty()) {
                std::cout << "For file " << filename << " was found files:\n";
            }
            for (const auto& found : found_vector) {
                std::cout << '\t' << found.file_path << '\n';
            }
            std::cout << '\n';
        } 
        for (const auto& filename : cmd.extensions_to_find) {
            std::vector<FileEntry> found_vector = searchByExtension(loaded, filename);
            if (!found_vector.empty()) {
                std::cout << "For file " << filename << " was found files:\n";
            }
            for (const auto& found : found_vector) {
                std::cout << '\t' << found.file_path << '\n';
            }
            std::cout << '\n';
        }
        for (const auto& filename : cmd.size_to_find) {
            std::vector<FileEntry> found_vector = searchBySize(loaded, filename);
            if (!found_vector.empty()) {
                std::cout << "For file " << filename << " was found files:\n";
            }
            for (const auto& found : found_vector) {
                std::cout << '\t' << found.file_path << '\n';
            }
            std::cout << '\n';
        }
    }
}

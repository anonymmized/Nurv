#include "app.hpp"
#include "scanner.hpp"
#include "types.hpp"

#include <vector>
#include <iostream>

void App::run() {
    if (cmd.argument == Arguments::Scan) {
        std::vector<FileEntry> scanned = scan(cmd.targetDirectory);
        if (scanned.empty()) {
            throw std::runtime_error("There is no files in directory");
        }
        for (const auto& file : scanned) {
            std::cout << "Filename: " << file.name << '\n';
            std::cout << "\tFilepath: " << file.file_path << '\n';
            std::cout << "\tExtension: " << file.extension << '\n';
            std::cout << "\tSize: " << file.size << "\n\n";
        }
    }
}

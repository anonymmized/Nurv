#include "parse_cmd_line.hpp"

#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <string>

namespace {
    struct SearchFlags {
        bool names = false;
        bool extensions = false;
        bool path = false;
        bool size = false;
    };
};

ParsedCmd parseCommandLine(int argc, char** argv) {
    if (argc < 2) {
        throw std::invalid_argument("Not enough arguments");
    }
    static const std::unordered_map<std::string, Arguments> commands = {
        {"--scan", Arguments::Scan},
        {"-sc", Arguments::Scan},
        {"--search", Arguments::Search},
        {"--sr", Arguments::Search},
        {"-h", Arguments::Help},
        {"--help", Arguments::Help}
    };
    auto it = commands.find(argv[1]);
    if (it == commands.end()) {
        throw std::invalid_argument("There is no argument: " + std::string(argv[1]));
    }
    ParsedCmd cmd;
    cmd.argument = it->second;
    if (cmd.argument == Arguments::Help) {
        std::cout << "Help page\n";
        throw;
    }
    if (cmd.argument == Arguments::Scan && argc < 3) {
        throw std::invalid_argument("Usage: nurv --scan <path>");
    }
    if (cmd.argument == Arguments::Search && argc < 3) {
        throw std::invalid_argument("Usage: nurv --search <target_file>");
    }
    if (argc >= 3 && cmd.argument == Arguments::Scan) {
        for (int i = 2; i < argc; i++) {
            std::filesystem::path target = std::string(argv[i]);
            if (!std::filesystem::exists(target)) {
                throw std::invalid_argument("The path " + std::string(argv[i]) + " not found");
            }
            cmd.targetDirectory.push_back(std::string(argv[i]));
        }
    } else if (argc > 3 && cmd.argument == Arguments::Search) {
        SearchFlags flags;
        for (int i = 2; i < argc; i++) {
            if (std::string(argv[i]) == "--name") {
                flags.names = true;
                flags.extensions = false;
                flags.path = false;
                flags.size = false;
                continue;
            } else if (std::string(argv[i]) == "--ext") {
                flags.extensions = true;
                flags.names = false;
                flags.path = false;
                flags.size = false;
                continue;
            } else if (std::string(argv[i]) == "--size") {
                flags.size = true;
                flags.extensions = false;
                flags.path = false;
                flags.names = false;
                continue;
            } else if (std::string(argv[i]) == "--path") {
                flags.path = true;
                flags.extensions = false;
                flags.size = false;
                flags.names = false;
                continue;
            } 
            if (flags.names) {
                cmd.names_to_find.push_back(std::string(argv[i]));
            }
            if (flags.extensions) {
                cmd.extensions_to_find.push_back(std::string(argv[i]));
            }
            if (flags.path) {
                cmd.path_to_find.push_back(std::string(argv[i]));
            }
            if (flags.size) {
                cmd.size_to_find.push_back(std::string(argv[i]));
            }
        }
    }
    return cmd;
}

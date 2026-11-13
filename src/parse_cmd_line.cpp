#include "parse_cmd_line.hpp"

#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <string>

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
    if (argc >= 3) {
        for (int i = 2; i < argc; i++) {
            std::filesystem::path target = std::string(argv[i]);
            if (!std::filesystem::exists(target)) {
                throw std::invalid_argument("The path " + std::string(argv[i]) + " not found");
            }
            cmd.targetDirectory.push_back(std::string(argv[i]));
        }
    }
    return cmd;
}

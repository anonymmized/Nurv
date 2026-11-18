#pragma once

#include <vector>

enum class Arguments {
    Scan,
    Search,
    Help
};

struct ParsedCmd {
    Arguments argument;
    std::vector<std::string> targetDirectory;
    std::vector<std::string> names_to_find;
};

ParsedCmd parseCommandLine(int argc, char** argv);


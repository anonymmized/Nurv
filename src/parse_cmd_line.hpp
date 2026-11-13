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
};

ParsedCmd parseCommandLine(int argc, char** argv);


#pragma 

#include "parse_cmd_line.hpp"
#include "types.hpp"

class App {
    private:
        ParsedCmd cmd;
        void processAllSearches(const std::vector<FileEntry>& loaded);
    public:
        App(int argc, char** argv) {
            cmd = parseCommandLine(argc, argv);
        }
        void run();
};

#pragma 

#include "parse_cmd_line.hpp"

class App {
    private:
        ParsedCmd cmd;
    public:
        App(int argc, char** argv) {
            cmd = parseCommandLine(argc, argv);
        }
        void run();
};

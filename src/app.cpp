#include "app.hpp"

void App::run() {
    if (cmd.argument == Arguments::Scan) {
        scan(cmd.targetDirectory);
    }
}

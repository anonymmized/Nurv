#include "app.hpp"

#include <iostream>

int main(int argc, char** argv) {
    try {
        App app(argc, argv);
    } catch (std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << '\n';
    }
}

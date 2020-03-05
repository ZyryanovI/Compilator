#include <iostream>
#include <fstream>
#include "MiniJava.Lexer.h"

int main(int argc, char** argv) {
    std::cout << "Hello, World!" << std::endl;
    yyscan_t scanner;
    mjlex_init(&scanner);

    std::ifstream in(argv[1]);
    if (in) {
        in.seekg(0, std::ios::end);
        size_t len = in.tellg();
        in.seekg(0);
        std::string contents(len + 1, '\0');
        in.read(&contents[0], len);

        mj_scan_string(contents.data(), scanner);
    }



	
    return 0;
}

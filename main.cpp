#include <iostream>
#include "MiniJava.Lexer.h"

int main(int argc, char** argv) {
    std::cout << "Hello, World!" << std::endl;
    yyscan_t scanner;
    mjlex_init(&scanner);
    mj_scan_string(argv[1], scanner);
	
    return 0;
}

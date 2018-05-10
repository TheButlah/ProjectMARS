#include <iostream>

#include "../include/CLIRepl.h"

int main(int argc, char* argv[]) {
	if(argc == 1) {
		std::cout << "Usage: ./cli /path/to/config.ini" << std::endl;
		exit(0);
	}
    MARS::CLIRepl cli(argv[1]);
    cli.startCLI();
}
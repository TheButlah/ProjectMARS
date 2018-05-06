#include "../include/CLIRepl.h"

int main(int argc, char* argv[]) {
    MARS::CLIRepl cli(argv[1]);
    cli.startCLI();
}
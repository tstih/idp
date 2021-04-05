#include <iostream>

#include "cxx_argp/cxx_argp_parser.h"

int main(int argc, char *argv[]) {

    cxx_argp::parser parser;

    std::pair<std::ifstream, std::string> rom;
    parser.add_option({"rom", 'r', "<filename>", 0, "Name of partner rom file"}, rom);

    if (parser.parse(argc, argv)) {
		std::cerr << "parsing OK\n";
	} else {
		std::cerr << "there was an error - exiting\n";
		return 1;
	}

	std::cerr << "rom file " << rom.second << "\n";     
}
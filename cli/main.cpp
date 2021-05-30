#include <symbol/symbol.h>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char** argv) {
	using namespace std;
	using Cli = symbol::Hmi;

    ostringstream os;
    for (int i=1; i< argc; ++i) {
        os << argv[i] << ' ';
    }
    string cmdline=os.str();

    Cli::Section::init(cout, cerr, "0.0.1");
    Cli cli;
    cli.init(argv[0], "Symbol wallet.");
	//TODO: interactive shell
    return cli.exec(cmdline)?0:1;
}


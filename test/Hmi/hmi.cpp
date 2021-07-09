#include "hmi.h"
#include <symbol/core.h>

namespace symbol { namespace test { namespace hmi {

	using std::tuple;
	using std::string;

	tuple<int, string, string> exec_offline(const string& pipe, const string& cmdline) {
		using Cli = symbol::core::Hmi;
		ostringstream out;
		ostringstream err;
		istringstream in(pipe);
		Cli::Section::init(out, err, in, "0.0.0");
		Cli cli;
		cli.init("symbol-offline", "Symbol wallet - offline.");
		int r = cli.exec(cmdline)?0:1;
		return make_tuple(r, out.str(), err.str());
	}

}}}


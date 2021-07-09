#pragma once

#include <tuple>
#include <string>

namespace symbol { namespace test { namespace hmi {

	/// Returns tuple of [exit_code, stdout, stderr]
	std::tuple<int, std::string, std::string> exec_offline(const std::string& pipe, const std::string& cmdline);

}}}


/**
*** Copyright (c) 2016-2019, Jaguar0625, gimre, BloodyRookie, Tech Bureau, Corp.
*** Copyright (c) 2020-present, Jaguar0625, gimre, BloodyRookie.
*** All rights reserved.
***
*** This file is part of Catapult.
***
*** Catapult is free software: you can redistribute it and/or modify
*** it under the terms of the GNU Lesser General Public License as published by
*** the Free Software Foundation, either version 3 of the License, or
*** (at your option) any later version.
***
*** Catapult is distributed in the hope that it will be useful,
*** but WITHOUT ANY WARRANTY; without even the implied warranty of
*** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*** GNU Lesser General Public License for more details.
***
*** You should have received a copy of the GNU Lesser General Public License
*** along with Catapult. If not, see <http://www.gnu.org/licenses/>.
**/
#include "Main.h"
#include "../catapult/LoggingConfiguration.h"

namespace symbol { namespace core { namespace hmi {
	
	using c = hmi::Main;

	c::Main(): b(Params{
			flagdefHome(),
			flagdefVerbose(),
			flagdefOutput(),
			flagdefHideLabels(),
		}) {
	}

	c::Main(Params&&p): b(move(p)) {
		add(flagdefHome());
		add(flagdefVerbose());
		add(flagdefOutput());
		add(flagdefHideLabels());
	}

	c::~Main() {
	}

	void c::init(const string& nm, const string& dc) {
		b::init(nm, dc);
		set_handler([&](const Params& p, ostream& os) -> bool { return mainHandler(p, os); });
	}

	c::FlagDef c::flagdefHome() {
		return FlagDef{Home_Flag, "home", true, true, "~/.symbol", "Home directory."}; //TODO:PORTABLE default path to home dir
	}

	c::FlagDef c::flagdefVerbose() {
		return FlagDef{Verbose_Flag, "verbose", true, false, "text", "Print extra information about what the program is doing."};
	}

	c::FlagDef c::flagdefOutput() {
		return FlagDef{Output_Flag, "output", true, true, "text", "Output format. Value in (text json)"};
	}

	c::FlagDef c::flagdefHideLabels() {
		return FlagDef{HideLabels_Flag, "hide-labels", true, false, "", "Hide field names. (Only on text output mode)"};
	}

	bool c::mainHandler(const Params& p, ostream& os) {
		m_home = p.get(Home_Flag);
		if (p.is_set(Verbose_Flag)) {
			//catapult::utils::log::global_logger::set(trace);
		}
		m_json = p.get(Output_Flag) == "json";
		m_hideLabels = p.is_set(HideLabels_Flag);
		return true;
	}

	void c::pass1(ParamPath& v) {
		b::pass1(v);
	}

	namespace {
		string fmt_field(const string& x, int w) {
			ostringstream os;
			if (x.size() >= w) {
				os << x << ' ';
				return os.str();
			}
			os << x << string(w, ' ');
			return os.str().substr(0, w);
		}

		static constexpr int f1w = 15;
	}

	void c::kv_text(const vector<pair<string, string>>& kv, ostream& os) {
		for (auto&i:kv) {
			os << fmt_field(i.first, f1w) << i.second << '\n';
		}
	}

	void c::kv_json(const vector<pair<string, string>>& kv, ostream& os) {
		os << "{\n";
		int n=kv.size();
		for (auto&i:kv) {
			os << "  \"" << i.first << "\": \"" << i.second << "\"";
			if (--n > 0) os << ", ";
			os << '\n';
		}
		os << "}\n";
	}

	template<> string c::toString(const uint8_t& o) {
		ostringstream os;
		os << +o;
		return os.str();
	}

}}}


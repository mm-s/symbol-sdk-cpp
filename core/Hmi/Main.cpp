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

	c::Params c::defParams() {
		return Params{
			{Home_Flag, Home_Name, true, true, Home_Default, Home_Desc},
			{Verbose_Flag, Verbose_Name, true, false, Verbose_Default, Verbose_Desc},
			{Output_Flag, Output_Name, true, true, Output_Default, Output_Desc},
//			{HideLabels_Flag, HideLabels_Name, true, false, HideLabels_Default, HideLabels_Desc},
		};
	}

	c::Main(): b(defParams()) {
	}

	c::Main(Params&& p): b(move(defParams())) {
		add(move(p));
	}

	c::~Main() {
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

/*
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
*/

	namespace {
		void help_flag_output(ostream& os) {
		 	os << "Valid options for flag --output:\n";
			os << "  text      Output data using text lines.\n";
			os << "  etext     Output data using text lines (electronic processing).\n";
			os << "  json      Output data using json format.\n";
			os << "  ejson     Output data using json format (electronic processing).\n";
		}
	}


	void c::help_flag(const FlagDef& f, ostream& os) const {
		if (f.short_name == Output_Flag) {
			help_flag_output(os);
			return;
		}
		b::help_flag(f, os);
	}

	bool c::main(Params& p, bool last, ostream& os) {
		m_home = p.get(Home_Flag);
		if (p.is_set(Verbose_Flag)) {
			//catapult::utils::log::global_logger::set(trace);
		}
		if (p.get(Output_Flag)!="text" && p.get(Output_Flag)!="etext" && p.get(Output_Flag)!="json" && p.get(Output_Flag)!="ejson") {
			os << "Invalid flag Output\n";
			help_flag_output(os);
			return false;
		}
		auto otype=p.get(Output_Flag);
		m_json = (otype == "json" || otype == "ejson");
		m_compact = (otype == "ejson" || otype == "etext");
//		m_hideLabels = p.is_set(HideLabels_Flag);
		return true;
	}

	void c::init(const string& nm, const string& dc) {
		b::init(nm, dc);
		set_handler([&](Params& p, bool last, ostream& os) -> bool { return main(p, last, os); });
	}

}}}


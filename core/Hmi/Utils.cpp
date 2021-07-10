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
#include "Blob.h"
#include "Network.h"
#include "../Utils.h"

namespace symbol { namespace core { namespace hmi {
	
	using c = hmi::Utils;

	c::Params c::defParams() {
		return Params{
			{Data_Flag, Data_Name, false, true, Data_Default, Data_Desc},
		};
	}

	void c::help_flag(const FlagDef& f, ostream& os) const {
		if (f.short_name == Data_Flag) {
			os << "Input data.\n";
			return;
		}
		b::help_flag(f, os);
	}

	bool c::cmdHexDecode(Params& p, bool last, ostream& os) {
		auto r = core::Utils::fromHex(m_data);
		if (is_ko(r.first)) {
			os << r.first;
			return false;
		}
		os.write(reinterpret_cast<const char*>(r.second.data()), r.second.size());
		return true;
	}

	bool c::cmdHex(Params& p, bool last, ostream& os) {
		if (last) {
			os << "Input length: " << m_data.size() << " bytes.\n";
			auto r = core::Utils::isHex(m_data);
			if (is_ko(r)) {
				os << "Input is not in Hex format.\n";
			}
			else {
				os << "Input data is in Hex format. \n";
				os << "Decoded length: " << m_data.size()/2 << " bytes.\n";
			}
		}
		return true;
	}

	bool c::cmdMain(Params& p, bool last, ostream& os) {
		m_data = p.get(Data_Flag);
		if (m_data.empty()) {
			os << "Empty input.";
			return false;
		}
		return true;
	}

	ptr<c::Section> c::createSectionHexDecode() {
		auto s=new Section(Params{});
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return cmdHexDecode(p, last, os); });
		return s;
	}

	ptr<c::Section> c::createSectionHex() {
		auto s=new Section(Params{});
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return cmdHex(p, last, os); });
		s->add(CmdDef{Decode_Command, Decode_Command_Desc}, createSectionHexDecode());
		return s;
	}

	ptr<c::Section> c::createSectionMain() {
		auto s=new Section(defParams());
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return cmdMain(p, last, os); });
		s->add(CmdDef{Hex_Command, Hex_Command_Desc}, createSectionHex());
		return s;
	}

	void c::init(const string& nm, const string& dc) {
		b::init(nm, dc);
		add(CmdDef{Main_Command, Main_Command_Desc}, createSectionMain());
	}

	bool c::pass1(ParamPath& v, ostream& os) {
		if (!b::pass1(v, os)) return false;
		return true;
	}

}}}


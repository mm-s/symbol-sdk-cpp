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

namespace symbol { namespace core { namespace hmi {
	
	using c = hmi::Utils;

	c::Params c::defParams() {
		return Params{
//			{Blob_Flag, Blob_Name, true, true, Blob_Default, Blob_Desc},
		};
	}

	c::Utils(): b(defParams()) {
	}

	c::Utils(Params&&p): b(move(defParams())) {
		add(move(p));
	}

	c::~Utils() {
	}

	void c::help_flag(const FlagDef& f, ostream& os) const {
/*
		if (f.short_name == Blob_Flag) {
			os << "Input an Hex string representing a memory representation of a Symbol object (Transaction, Block, other)\n";
			os << "Example: --blob B000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000009398F182AE4B7A91B1348D607BF6F0B01D62A0F9BA1965AC19EFA190BF3A0529000000000178544100000000000000000000000000000000781DECC1FAD08371CC057F4194697CE92D2838E20FDD67E8000001000000000000000000000000000100000000000000\n";
			return;
		}
*/
		b::help_flag(f, os);
	}

	bool c::cmdMain(Params& p, bool last, ostream& os) {
		os << "Not implemented.";
		return true;
	}

	ptr<c::Section> c::createSectionMain() {
		auto s=new Section(defParams());
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return cmdMain(p, last, os); });
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


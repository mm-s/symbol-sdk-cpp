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
#include "NonFungibleToken.h"
#include "../../NonFungibleToken.h"

namespace symbol { namespace core { namespace hmi {
	using c = hmi::NonFungibleToken; /// Implementation for class c 

	void c::init(const string& name, const string& desc) {
		b::init(name, desc);
		add(CmdDef{NFT_Command, NFT_Command_Desc}, createSectionNft());
	}

	bool c::nftInfo(const Params& p, ostream& os) {
		os << "Info about NFT: not implemented." << '\n';
		return false; //Will result in Error for the user
	}

	ptr<c::Section> c::createSectionNftInfo() {
		auto s=new Section(Params{});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return nftInfo(p, os); });
		return s;
	}

	ptr<c::Section> c::createSectionNft() {
		auto s=new Section(Params{});
		s->add(CmdDef{Info_Command, Info_Command_Desc}, createSectionNftInfo());
		return s;
	}

}}}


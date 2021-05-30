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
#include "HmiKeys.h"

namespace symbol {

	using c = HmiKeys;

	symbol::keys::pub* c::resolve_public_key(const symbol::net::address& addr0) const {
		string a=addr0.format_account();
		dto::account_t o;
		auto r=o.fetch(getUrl(), a);
		if (!r) return nullptr;
		
		//public key, address
		symbol::keys::pub* pk{nullptr};
		symbol::net::address* addr{nullptr};
		string e=get_net().parse(o.account.publicKey, pk, addr);
		if (!e.empty()) {
		    return nullptr;
		}
		delete addr;
		return pk;    
	}

}


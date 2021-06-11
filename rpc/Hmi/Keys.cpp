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
#include "Keys.h"

namespace symbol { namespace hmi {

	using c = symbol::hmi::Keys;

	ptr<symbol::PublicKey> c::resolvePublicKey(const symbol::UnresolvedAddress& addr0) const {
		string a=addr0.formatAccount();
		dto::account_t o;
		auto r=o.fetch(url(), a);
		if (!r) return nullptr;
		
		//public key, address
		ptr<symbol::PublicKey> pk{nullptr};
		ptr<symbol::UnresolvedAddress> addr{nullptr};
		string e=network().parse(o.account.publicKey, pk, addr);
		if (!e.empty()) {
		    return nullptr;
		}
		delete addr;
		return pk;    
	}

}}


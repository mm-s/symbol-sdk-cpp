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
#include "Utils.h"
#include <vector>
#include <string>
#include "catapult/HexParser.h"

namespace symbol { namespace core {
	using c = Utils;

	pair<ko, vector<uint8_t>> c::fromHex(const string& hex) {
		vector<uint8_t> r;
		if (hex.size()&1) {
			return make_pair("KO 44932 Hex must be formed of an even number of digits.", move(r));
		}
		r.resize(hex.size()/2);
		if (!catapult::utils::TryParseHexStringIntoContainer(hex.data(), hex.size(), r)) {
			return make_pair("KO 50948 Cannot decode hex.", move(r));
		}
		return make_pair(ok, move(r));
	}

	ko c::isHex(const string& hex) {
		auto r=fromHex(hex);
		return r.first;
	}

}}


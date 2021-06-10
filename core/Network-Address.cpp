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

#include "Network.h"
#include "catapult/Address.h"

namespace symbol {
	/// Implementation for the class c 
	using c = Network::UnresolvedAddress;
	using std::move;
	using std::ostringstream;
	using std::make_pair;

	c::UnresolvedAddress(b&& a, const Network& n): b(a), m_network(n) {
	}

	c::UnresolvedAddress(const Network& n): m_network(n) {
	}

	c::UnresolvedAddress(UnresolvedAddress&& other): b(other), m_network(other.m_network) {
	}

	bool c::isValid() const {
		return m_network.isValid(*this);
	}

	bool c::isAddress(const string& hex) {
		catapult::Address x;
		return catapult::utils::TryParseHexStringIntoContainer(hex.data(), hex.size(), x);
	}

	string c::formatAccount() const {
		return catapult::model::AddressToString(m_network.resolver().resolve(*this));
	}

} // Namespace Symbol


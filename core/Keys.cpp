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
#include "catapult/SecureRandomGenerator.h"
#include "catapult/HexFormatter.h"
#include <cassert>
#include <sstream>

/// TODO use catapult-server/tools/tools/KeyValueOutputBuilder.h

namespace symbol { namespace core {
	using c = Keys; /// Implementation for the class c 
	using std::move;
	using std::ostringstream;
	using std::make_pair;

	c::Keys(PrivateKey&&sk): b(move(sk)) {
	}

	c::Keys(const PrivateKey& sk): b(copyPrivateKey(sk)) {
	}

	c::Keys(c&&other): b(move(other)) {
	}

	c::~Keys() {
	}

	const c::PublicKey& c::publicKey() const {
		return b::publicKey();
	}

	const c::PrivateKey& c::privateKey() const {
		return b::privateKey();
	}

	c::PrivateKey c::copyPrivateKey(const PrivateKey& other) {
		ostringstream os;
		os << other;
		return parsePrivateKey(os.str());
	}

	ptr<c::PrivateKey> c::newPrivateKey() {
		catapult::crypto::SecureRandomGenerator generator;
		return new PrivateKey(PrivateKey::Generate(generator));
	}

	c::PrivateKey c::generatePrivateKey() {
		catapult::crypto::SecureRandomGenerator generator;
		return PrivateKey::Generate(generator);
	}

	c::PrivateKey c::parsePrivateKey(const string& hex) {
		try {
			return PrivateKey::FromString(hex);
		}
		catch(...) {
			return PrivateKey();
		}
	}

	ptr<c::PrivateKey> c::createPrivateKey(const string& hex) {
		try {
			return new PrivateKey(PrivateKey::FromString(hex));
		}
		catch(...) {
			return nullptr;
		}
	}

	ptr<c::PublicKey> c::createPublicKey(const PrivateKey& sk) {
		ptr<PublicKey> pk = new PublicKey();
		try {
			catapult::crypto::Ed25519KeyPairTraits::ExtractPublicKeyFromPrivateKey(sk, *pk);
		}
		catch(...) {
			return nullptr;
		}
		return pk;
	}

	ptr<c::PublicKey> c::createPublicKey(const string& hex) {
		ptr<PublicKey> x = new PublicKey();
		if (!catapult::utils::TryParseHexStringIntoContainer(hex.data(), hex.size(), *x)) {
			delete x;
			return nullptr;
		}
		return x;
	}

	bool c::isKey(const string& hex) {
		assert(sizeof(PublicKey) == sizeof(PrivateKey));
		PublicKey x;
		return catapult::utils::TryParseHexStringIntoContainer(hex.data(), hex.size(), x);
	}

	pair<ko, c> c::generate() {
		return move( make_pair(ok, generatePrivateKey()) );
	}

	pair<ko, c> c::generate(const string& sk_hex) {
		PrivateKey sk = PrivateKey::FromString(sk_hex);
		return move( make_pair(ok, c(move(sk))) );
	}

	void c::to_stream(ostream& os) const {
		using namespace catapult::utils;
		os << "Private Key: " << HexFormat(m_privateKey) << '\n';
		os << "Public Key: " << m_publicKey << '\n';
	}

}} // Namespaces


std::ostream& operator << (std::ostream& os, const symbol::core::Keys& o) {
	o.to_stream(os);
	return os;
}

std::ostream& operator << (std::ostream& os, const symbol::core::Keys::PrivateKey& o) {
	os << HexFormat(o);
	return os;
}


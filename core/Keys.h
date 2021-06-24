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
#pragma once
#include "base.h"
#include <map>
#include <iostream>
#include "catapult/types.h"
#include "catapult/KeyPair.h"

namespace symbol { namespace core {
	using std::string;
	using std::pair;
	using std::ostream;

	/// Functions related to Cryptographic Keys
	class Keys: public catapult::crypto::KeyPair {
		/// Base class b
		using b = catapult::crypto::KeyPair;

	public:
		/// Construction, Initialization, Destruction
		Keys(PrivateKey&&);
		Keys(const PrivateKey&);
		Keys(Keys&&);
		~Keys();

	public:
		/// Private Key Production
		static PrivateKey generatePrivateKey();
		static ptr<PrivateKey> newPrivateKey();
		static ptr<PrivateKey> createPrivateKey(const string& hex);
		static PrivateKey parsePrivateKey(const string& hex);
		static PrivateKey copyPrivateKey(const PrivateKey&);

		/// Public Key Production
		static ptr<PublicKey> createPublicKey(const PrivateKey& sk);
		static ptr<PublicKey> createPublicKey(const string& hex);

		/// KeyPair generation
		static pair<ko, Keys> generate();

		/// KeyPair generation from private key
		static pair<ko, Keys> generate(const string& privateKeyHex);

	public:
		/// Validation
		static bool isKey(const string& hex);

	public:
		/// Getters and Setters
		const PublicKey& publicKey() const;
		const PrivateKey& privateKey() const;

	public:
		/// Serialization
		void to_stream(ostream&) const;

	};

	/// Types published on the main namespace
	//using PublicKey = Keys::PublicKey;
	//using PrivateKey = Keys::PrivateKey;

}} // namespaces

namespace symbol { //publish on namespace symbol
	using Keys = ::symbol::core::Keys;
	using PublicKey = Keys::PublicKey;
	using PrivateKey = Keys::PrivateKey;
}


/// Streaming overloads
std::ostream& operator << (std::ostream&, const symbol::core::Keys&);
std::ostream& operator << (std::ostream&, const symbol::core::Keys::PrivateKey&);


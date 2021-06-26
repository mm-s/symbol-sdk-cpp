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
#include "Keys.h"
#include "Transaction.h"
#include "catapult/types.h"
#include "catapult/Transaction.h"
#include "catapult/ResolverContext.h"
#include <iostream>

namespace symbol { namespace core {

	class Transfer;
	
	/// Instance covering identified networks.
	class Network {
	public:
		using Identifier = Transaction::NetworkIdentifier;
		using GenerationHashSeed = catapult::GenerationHashSeed;
		using ResolverContext = catapult::model::ResolverContext;
		using Blob = Transaction::Blob;
	public:
		/// Address tied to a Network instance
		class UnresolvedAddress: public catapult::UnresolvedAddress {
			/// Base class
			using b = catapult::UnresolvedAddress;

		public:
			/// Construction, Initialization, Destruction
			UnresolvedAddress(const Network&);
			UnresolvedAddress(b&&, const Network&);
			UnresolvedAddress(UnresolvedAddress&&);

			

			/// Validation
			bool isValid() const;
			static bool isAddress(const string& hex);

			/// Formatting
			string formatAccount() const;

		private:
			const Network& m_network;
		};

	public:
		class Account: UnresolvedAddress {
			public:
		};

	public:
		/// Construction, Initialization, Destruction
		Network(const Identifier&);
		Network(const string& identifierName);
		virtual ~Network();

	public:
		/// List valid network Identifiers
		static void list(ostream& os);
		static void list(const string& prefix, ostream& os);

		/// Determine if the given Identifier is among the listed ones.
		static bool listed(const Identifier&);

		/// Node list source
		string nodesUrl() const;

	public:
		/// Parsing NetworkIdentifier from name(e.g. "public")/ Hex string, or Formatted Account
		static Identifier identifierFromName(const string& name);
		static Identifier identifierFromAddressHex(const string& hex);
		static Identifier identifierFromAccount(const string& enc);
		static Identifier identifierFromEntity(const string& memHex);

		/// Obtain PublicKey and/or Address from a string representing either an Hex PublicKey, an Hex Address or a formatted Account.
		/// Fails when the network identifier encoded in the address -if any- doesn't match this instance Identifier.
		/// Returns empty string on success, or error message.
		string parse(const string& input, ptr<PublicKey>&, ptr<UnresolvedAddress>&) const;

		/// Resets this instance's Identifier with the network identifier encoded in the address -if any-.
		string parse(const string& input, ptr<PublicKey>&, ptr<UnresolvedAddress>&, bool as_const);

	public:
		/// Generation of a new address
		[[nodiscard]] ptr<UnresolvedAddress> newAddress() const;

		/// New Address from Public Key
		[[nodiscard]] ptr<UnresolvedAddress> newAddress(const PublicKey&) const;

		/// New Address from Key Pair
		[[nodiscard]] ptr<UnresolvedAddress> newAddress(const Keys& k) const { return newAddress(k.publicKey()); }

		/// New Address from Hex string
		[[nodiscard]] ptr<UnresolvedAddress> newAddress(const string& hex) const;

		/// New Address from formatted Account
		[[nodiscard]] ptr<UnresolvedAddress> newAccount(const string& enc) const;

	public:
		/// Validation
		bool isValid(const UnresolvedAddress&) const;
		bool isValidIdentifier() const;
		static bool isValid(const Identifier&);

	public: /// Serialization
		/// Write state to output stream
		void toStream(ostream&) const;

	public: /// Getters and Setters
		/// Set network GenerationHashSeed
		bool setSeed(const string&);
		string identifierHex() const;
		string identifierDecHex() const;
		const char* identifierStr() const;
		static const char* identifierStr(Identifier t);
		static Identifier identifier(const string& name); // can return Identifier::Zero
		static Identifier identifier(const Blob&); // can return Identifier::Zero
		inline const Identifier& identifier() const { return m_identifier; }
		inline const GenerationHashSeed& seed() const { return m_seed; }
		inline const ResolverContext& resolver() const { return m_resolver; }

		static pair<ko, Blob> decodeBlob(const string& Hex);

	public:	/// Transactions
		/// Creates a Transfer transaction
		pair<ko, ptr<Transaction>> createTransaction(const Blob& mem) const;
		pair<ko, ptr<Transfer>> createTransfer(const UnresolvedAddress& rcpt, const Amount& a, const MosaicId& m, const Amount& f, const TimeSpan& d, const vector<uint8_t>& msg, const ptr<PrivateKey>& encryptPrivateKey=nullptr, const ptr<PublicKey>& encryptPublicKey=nullptr) const;


	private:
		Identifier m_identifier;
		GenerationHashSeed m_seed;
		ResolverContext m_resolver;

	};

}} // namespaces

namespace symbol { //publish on namespace symbol
	using Network = symbol::core::Network;
	using UnresolvedAddress = Network::UnresolvedAddress;
}

std::ostream& operator << (std::ostream&, const symbol::core::Network&);


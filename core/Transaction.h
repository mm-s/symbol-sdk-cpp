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
#include "catapult/Transaction.h"
#include "catapult/ConfigurationValueParsers.h"
#include "catapult/NetworkIdentifier.h"
#include "catapult/Mosaic.h"
#include "catapult/TimeSpan.h"

namespace symbol { namespace core {
	using namespace std;

	class Network;

	/// Class Transaction. Just making space for the comment.
	class Transaction {
	public:
		using UnresolvedAddress = catapult::UnresolvedAddress;
		using Amount = catapult::Amount;
		using Timestamp = catapult::Timestamp;
		using TimeSpan = catapult::utils::TimeSpan;
		using Msg = vector<uint8_t>;
		using NetworkIdentifier = catapult::model::NetworkIdentifier;

		/// Class Mosaic. Just making space for the comment.
		class Mosaic: public catapult::model::Mosaic {
			using b = catapult::model::Mosaic;

		public:
			using Id = catapult::MosaicId;
			using UnresolvedId = catapult::UnresolvedMosaicId;

			uint64_t amount() const { return Amount.unwrap(); }
			uint64_t id() const { return MosaicId.unwrap(); }

			void toStream(ostream&) const;
		};

		Transaction(const Network&, catapult::model::Transaction*);
		Transaction(Transaction&&);
		virtual ~Transaction();

		static pair<ko, ptr<Transaction>> create(const string& memHex);


		template<typename t>
		static bool parse(const string& input, t&o) {
			try {
				catapult::utils::TryParseValue(input, o);
				return true;
			}
			catch(...) {
				return false;
			}
		}

		bool sign(const Keys::PrivateKey&);

		ptr<catapult::model::Transaction> m_catapultTx{ nullptr };
		const Network& m_network;
	};

	class Transfer: public Transaction {
		using b = Transaction;

	public:
		Transfer(const Network&, ptr<catapult::model::Transaction>);
		Transfer(Transfer&&);
		bool toStream(ostream&) const;

		static ptr<Transfer> create(const Network&, const UnresolvedAddress& rcpt, const Amount&, const Mosaic::Id&, const Amount& maxfee, const TimeSpan& deadline, const vector<uint8_t>& msg);
		ptr<catapult::model::Transaction> m_catapultTransferTx{ nullptr };


	};

}} //Namespaces

namespace symbol { //publish on namespace symbol
	using Transaction = ::symbol::core::Transaction;
	using UnresolvedMosaicId = Transaction::Mosaic::UnresolvedId;
	using MosaicId = Transaction::Mosaic::Id;
	using Amount = Transaction::Amount;
	using Timestamp = Transaction::Timestamp;
	using TimeSpan = Transaction::TimeSpan;
	
}
std::ostream& operator << (std::ostream&, const symbol::core::Transaction::Mosaic&);
std::ostream& operator << (std::ostream&, const symbol::core::Transfer&);


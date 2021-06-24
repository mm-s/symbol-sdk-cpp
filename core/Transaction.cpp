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

#include "Transaction.h"
#include "Transaction/Transfer.h"
#include "Network.h"
#include "catapult/BufferInputStreamAdapter.h"
#include "catapult/EntityIoUtils.h"
#include "catapult/TransactionExtensions.h"
/*
#include "catapult/TransactionBuilder.h"
#include "catapult/TransferBuilder.h"
#include "catapult/HexFormatter.h"
#include "catapult/RawBuffer.h"
#include <vector>
#include <type_traits>
*/

namespace symbol { namespace core {
	/// Implementation for the class c 
	using c = core::Transaction;
	using std::move;
	using std::ostringstream;
	using std::make_pair;

	//./catapult-server/tests/test/local/RealTransactionFactory.cpp
	c::Transaction(const Network& n, catapult::model::Transaction* u): m_network(n), m_catapultTx(u) {

	}

	c::Transaction(Transaction&& other): m_catapultTx(other.m_catapultTx), m_network(other.m_network) {
		other.m_catapultTx = nullptr;
	}

	c::~Transaction() {
		delete m_catapultTx;
	}

/*
utils::NonCopyable
	uint32_t Size; /// Entity size.
SizePrefixedEntity
	uint32_t VerifiableEntityHeader_Reserved1;
	catapult::Signature Signature;
VerifiableEntityHeader
	Key SignerPublicKey;
	uint32_t EntityBody_Reserved1;
	uint8_t Version;
	NetworkIdentifier Network;
	EntityType Type;
EntityBody
VerifiableEntity
	Amount MaxFee; /// Maximum transaction fee paid for confirmation./// \note Actual fee paid is dependent on containing block.
	Timestamp Deadline; /// Transaction deadline.
Transaction

*/
	bool c::isTransaction(const Blob& blob) {
		auto is = catapult::io::BufferInputStreamAdapter(blob);
		try {
			auto ptx = catapult::io::ReadEntity<catapult::model::Transaction>(is);
			return catapult::model::ToBasicEntityType(ptx->Type) == catapult::model::BasicEntityType::Transaction;
		}
		catch (...) {
			///CATAPULT_THROW_INVALID_ARGUMENT("size is insufficient");
		}
		return false;
	}

	bool c::isTransferTransaction(const Blob& blob) {
		auto is = catapult::io::BufferInputStreamAdapter(blob);
		try {
			auto ptx = catapult::io::ReadEntity<catapult::model::TransferTransaction>(is);
			return ptx->Type == catapult::model::Entity_Type_Transfer;
			
		}
		catch (...) {
		}
		return false;
	}

	pair<ko, ptr<Transaction>> c::create(const Network& n, const Blob& blob) {
		if (isTransferTransaction(blob)) {
			auto r=Transfer::create(n, blob);
			if (is_ko(r.first)) {
				assert(r.second==nullptr);
				return make_pair(r.first, nullptr);
			}
			return make_pair(ok, r.second);
		}
		return make_pair("KO 66095", nullptr);
	}

	ko c::sign(const Keys::PrivateKey& sk) {
		if (m_catapultTx == nullptr ) {
			return "KO 99483";
		}
		
		//cout << m_network.seed() << endl;
		auto x = catapult::extensions::TransactionExtensions(m_network.seed());
		Keys k(sk);
		m_catapultTx->SignerPublicKey=k.publicKey();

		x.sign(k, *m_catapultTx);
		if (!x.verify(*m_catapultTx)) {
//			return "KO 66051";
			cout << "KO 66051" << endl;
		}
		return ok;
	}

}} // namespaces



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
#include "Network.h"
#include "catapult/TransactionBuilder.h"
#include "catapult/TransferBuilder.h"
#include "catapult/HexFormatter.h"
#include "catapult/RawBuffer.h"
#include "catapult/EntityIoUtils.h"
#include "catapult/BufferInputStreamAdapter.h"
#include "catapult/TransactionExtensions.h"
#include <vector>
#include <type_traits>


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

	

	pair<ko, ptr<Transfer>> Transfer::create(const Network& n, const Blob& mem) {
		auto is = catapult::io::BufferInputStreamAdapter(mem);

	//	catapult::model::IsSizeValidT<catapult::model::Transaction>
	//	EntityType type=
	//	BasicEntityType bet=catapult::model::ToBasicEntityType(entity.Type);
		std::unique_ptr<catapult::model::TransferTransaction> ptx;
		try {
			ptx = catapult::io::ReadEntity<catapult::model::TransferTransaction>(is);
			assert(ptx->Type == catapult::model::Entity_Type_Transfer);
		}
		catch (...) {
			///CATAPULT_THROW_INVALID_ARGUMENT("size is insufficient");
			return make_pair("KO 84039", nullptr);
		}
		if (n.identifier() != ptx->Network) {
			return make_pair("KO 82291", nullptr);
		}
		return make_pair(ok, new Transfer(n, ptx.release()));
	}




	Transfer::Transfer(const Network& n, ptr<catapult::model::TransferTransaction> t): m_catapultTransferTx(t), b(n, t) {
	}

	Transfer::Transfer(Transfer&& other): b(move(other)), m_catapultTransferTx(other.m_catapultTransferTx) {
		other.m_catapultTransferTx = nullptr;
	}


	pair<ko, ptr<Transfer>> Transfer::create(const Network& n, const UnresolvedAddress& rcpt, const Amount& am,  const Mosaic::Id& m, const Amount& maxfee, const TimeSpan& deadline, const Msg& msg) {
		auto k=Keys::generate();
		catapult::builders::TransferBuilder builder(n.identifier(), k.second.publicKey());
		if (!msg.empty()) {
			builder.setMessage(catapult::utils::RawBuffer(msg.data(), msg.size()));
		}
		builder.setRecipientAddress(rcpt);
	//	for (const auto& seed : seeds) {
	//		auto mosaicId = mosaicNameToMosaicIdMap.at(seed.Name);
	//		builder.addMosaic({ mosaicId, seed.Amount });
	//	}
		UnresolvedMosaicId um(m.unwrap());
		builder.addMosaic({ um, am });
		builder.setDeadline(Timestamp(deadline.millis()));
		auto x = builder.build().release();
//cout << "XXXXXXXXXXXXXXXXXXXX" << endl;
//cout << x->Network << " " << x->Size << " bytes" << endl;
		
		return make_pair(ok, new Transfer(n, x));
	}

	bool c::sign(const Keys::PrivateKey& sk) {
		if (m_catapultTx != nullptr ) return false;
		auto x = catapult::extensions::TransactionExtensions(m_network.seed());
		Keys k(sk);
		x.sign(k, *m_catapultTx);
		return x.verify(*m_catapultTx);
	}

	/*
	bool c::parse(const string& input, amount&o) {
		try {
		    catapult::utils::TryParseValue(input, o);
		    return true;
		}
		catch(...) {
		    return false;
		}
	}
	*/

	/*
	#include "core.h"

	using namespace std;

	typedef symbol::transaction c;

	c::~transaction() {

	}
	*/

	bool Transfer::toStream(ostream& os) const {
		if (m_catapultTx==nullptr) {
			return false;
		}
		Blob buf;
		buf.resize(m_catapultTx->Size);
		memcpy(buf.data(), m_catapultTx, m_catapultTx->Size);
		os << catapult::utils::HexFormat(buf);
		return true;
	}

}} // namespaces

std::ostream& operator << (std::ostream& os, const symbol::core::Transfer& o) {
	o.toStream(os);
	return os;
}


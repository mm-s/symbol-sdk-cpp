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
#include "catapult/TransactionExtensions.h"
#include <vector>

namespace symbol { namespace core {
	/// Implementation for the class c 
	using c = Transaction;
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

	Transfer::Transfer(const Network& n, ptr<catapult::model::Transaction> t): b(n, t) {
	}

	Transfer::Transfer(Transfer&& other): b(move(other)) {
	}

	ptr<Transfer> Transfer::create(const Network& n, const UnresolvedAddress& rcpt, const Amount& am,  const Mosaic::Id& m, const Amount& maxfee, const TimeSpan& deadline) {
		auto k=Keys::generate();
		catapult::builders::TransferBuilder builder(n.identifier(), k.second.publicKey());
		
		builder.setRecipientAddress(rcpt);
	//	for (const auto& seed : seeds) {
	//		auto mosaicId = mosaicNameToMosaicIdMap.at(seed.Name);
	//		builder.addMosaic({ mosaicId, seed.Amount });
	//	}
		UnresolvedMosaicId um(m.unwrap());
		builder.addMosaic({ um, am });
		builder.setDeadline(Timestamp(deadline.millis()));
		auto x=builder.build().release();
		return new Transfer(n, x);
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

	void Transfer::toStream(ostream& os) const {
		auto sz=sizeof(*m_catapultTx);
		vector<char> buf;
		buf.resize(sz);
	//    char buf[sz];
		memcpy(buf.data(), m_catapultTx, sz);
		os << catapult::utils::HexFormat(buf) << endl;

	//    os << "tx: sz " << sizeof(*u) << endl;
	}

}} // namespaces

std::ostream& operator << (std::ostream& os, const symbol::core::Transfer& o) {
	o.toStream(os);
	return os;
}


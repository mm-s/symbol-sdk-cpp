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

#include "Transfer.h"
#include "../catapult/BufferInputStreamAdapter.h"
#include "../catapult/EntityIoUtils.h"
#include "../Network.h"
#include "../catapult/TransferBuilder.h"

namespace symbol { namespace core {
	/// Implementation for the class c 
	using c = core::Transfer;
	using std::move;
	using std::ostringstream;
	using std::make_pair;

	c::Transfer(const Network& n, ptr<catapult::model::TransferTransaction> t): m_catapultTransferTx(t), b(n, t) {
	}

	c::Transfer(Transfer&& other): b(move(other)), m_catapultTransferTx(other.m_catapultTransferTx) {
		other.m_catapultTransferTx = nullptr;
	}

	pair<ko, ptr<Transfer>> c::create(const Network& n, const Blob& mem) {
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

	pair<ko, ptr<Transfer>> c::create(const Network& n, const UnresolvedAddress& rcpt, const Amount& am,  const Mosaic::Id& m, const Amount& maxfee, const TimeSpan& deadline, const Msg& msg) {
		PublicKey unused;
//		auto k=Keys::generate();
		catapult::builders::TransferBuilder builder(n.identifier(), unused);
		if (!msg.empty()) {
			builder.setMessage(catapult::utils::RawBuffer(msg.data(), msg.size()));
		}
		builder.setRecipientAddress(rcpt);
	//	for (const auto& seed : seeds) {
	//		auto mosaicId = mosaicNameToMosaicIdMap.at(seed.Name);
	//		builder.addMosaic({ mosaicId, seed.Amount });
	//	}
cout << "xXXXXXXXXXXXXXX " << m.unwrap() << endl;
		UnresolvedMosaicId um(1); //m.unwrap());
		builder.addMosaic({ um, am });
		builder.setDeadline(Timestamp(54321)); //deadline.millis()));
		auto x = builder.build().release();
//cout << "XXXXXXXXXXXXXXXXXXXX" << endl;
//cout << x->Network << " " << x->Size << " bytes" << endl;
		
		return make_pair(ok, new Transfer(n, x));
	}

	bool c::toStream(ostream& os) const {
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


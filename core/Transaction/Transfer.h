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

#include "../Transaction.h"

namespace symbol { namespace core {

	class Transfer: public Transaction {
		using b = Transaction;
	public:
		Transfer(const Network&, ptr<catapult::model::TransferTransaction>);
		Transfer(Transfer&&);
		bool toStream(ostream&) const;
		static pair<ko, ptr<Transfer>> create(const Network& n, const Blob& mem);
		static pair<ko, ptr<Transfer>> create(const Network&, const UnresolvedAddress& rcpt, const Amount&, const Mosaic::Id&, const Amount& maxfee, const TimeSpan& deadline, const Msg& msg);

		ptr<catapult::model::TransferTransaction> m_catapultTransferTx{ nullptr };
	};

}} //Namespaces

namespace symbol { //publish on namespace symbol
	using Transfer = ::symbol::core::Transfer;
	
}

std::ostream& operator << (std::ostream&, const symbol::core::Transfer&);


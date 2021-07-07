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

#include <symbol/core/Hmi/Transaction/Transfer.h>
#include "../Transaction.h"

namespace symbol { namespace hmi {

	/// Human-Machine Interface. Transactions (tx) section command processor (online+offline)
	class Transfer: public symbol::core::hmi::Transfer {
		using b = symbol::core::hmi::Transfer;

	public:
		using b::Transfer;

		bool main(Params&, bool last, std::ostream&) override;
		//ptr<symbol::PublicKey> resolvePublicKey(const symbol::UnresolvedAddress&) const override;
		
	protected:
		Transaction* root();
		const Transaction* root() const;

		///Opportunity to rewrite Params before command execution.
		bool pass1(ParamPath&, ostream&) override;

	private:
		//ptr<Section> createSectionTxTransfer() override;



	};

}}


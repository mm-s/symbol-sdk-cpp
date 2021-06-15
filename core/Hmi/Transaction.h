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

#include "Keys.h"
#include "../Transaction.h"

namespace symbol { namespace core { namespace hmi {

	/// Human-Machine Interface. Transaction(tx) section command processor (offline)
	class Transaction: public hmi::Keys {
		/// Base class b.
		using b = hmi::Keys;

	public:
		static constexpr const char* TX_Command = "tx";
		static constexpr const char* TX_Command_Desc = "Operations with transactions.";
	
		static constexpr const char* Transfer_Command = "transfer";
		static constexpr const char* Transfer_Command_Desc = "Transfer transaction.";

		/// Flags, Options
		static constexpr char Privkey_Flag{'s'};
		static constexpr char Recipient_Flag{'r'};
		static constexpr char Amount_Flag{'a'};
		static constexpr char Mosaic_Flag{'m'};
		static constexpr char Maxfee_Flag{'f'};
		static constexpr char Deadline_Flag{'d'};
	public:		
		/// Default constructors
		using b::Keys;

		/// Provide the program name and a description.
		void init(const string& name, const string& desc) override;

	private:
		/// menu command: tx
		ptr<Section> createSectionTx();

	protected: /// menu command: tx transfer
		ptr<Section> createSectionTxTransfer(); /// Init
		virtual bool txTransfer(const Params&, ostream&); /// Command Handler
		
	};

}}}


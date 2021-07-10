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

#include "Blob.h"
#include "../Transaction.h"

namespace symbol { namespace core { namespace hmi {

	/// Human-Machine Interface. Transaction(tx) section command processor (offline)
	class Transaction: public hmi::Blob {
		/// Base class b.
		using b = hmi::Blob;

	public:
		static constexpr auto Main_Command = "tx";
		static constexpr auto Main_Command_Desc = "Operations with transactions.";

		static constexpr auto Info_Command = "info";
		static constexpr auto Info_Command_Desc = "Info about transaction.";

		static constexpr const char* Transfer_Command = "transfer";
		static constexpr const char* Transfer_Command_Desc = "Transfer transaction.";

	public:
		/// Default constructors
		using b::Blob;

		//~Transaction() override;

		/// Provide the program name and a description.
		void init(const string& name, const string& desc) override;
		bool pass1(ParamPath&, ostream&) override;

	public:
		virtual ptr<Section> createSectionTransfer(); /// Init

	private:
		/// menu command: tx
		ptr<Section> createSectionMain();

	protected: /// menu command: tx transfer
		bool cmdMain(Params&, bool last, ostream&); /// Command Handler

		//virtual bool txTransfer(Params&, ostream&); /// Command Handler
	
	private:
		//core::Transaction* m_tx;
	};

}}}


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
#include "../conch.h"

namespace symbol { namespace core { namespace hmi {

	/// Human-Machine Interface. Transaction(tx) section command processor (offline)
	class Transfer: public conch::section {
		/// Base class b.
		using b = conch::section;

	public:
		using Section = conch::section;
		using Params = conch::params;
		using CmdDef = conch::cmddef;
		using FlagDef = conch::flagdef;
		using ParamPath = b::param_path;
		using ostream = std::ostream;
	public:
	
		static constexpr const char* Transfer_Command = "transfer";
		static constexpr const char* Transfer_Command_Desc = "Transfer transaction.";

		static Params defParams();

		/// Flags, Options
		//static constexpr char Privkey_Flag{'s'};
		static constexpr auto Recipient_Flag{'r'};
		static constexpr auto Recipient_Name{"recipient"};
		static constexpr auto Recipient_Default{""};
		static constexpr auto Recipient_Desc{"Recipient address."};

		static constexpr auto Amount_Flag{'a'};
		static constexpr auto Amount_Name{"amount"};
		static constexpr auto Amount_Default{"0"};
		static constexpr auto Amount_Desc{"Amount of mosaics."};

		static constexpr auto Mosaic_Flag{'m'};
		static constexpr auto Mosaic_Name{"mosaic"};
		static constexpr auto Mosaic_Default{""};
		static constexpr auto Mosaic_Desc{"Mosaic id."};

		static constexpr auto Maxfee_Flag{'f'};
		static constexpr auto Maxfee_Name{"maxfee"};
		static constexpr auto Maxfee_Default{""};
		static constexpr auto Maxfee_Desc{"Maximum fee to pay."};

		static constexpr auto Deadline_Flag{'d'};
		static constexpr auto Deadline_Name{"deadline"};
		static constexpr auto Deadline_Default{""};
		static constexpr auto Deadline_Desc{"Deadline."};

		static constexpr auto Message_Flag{'M'};
		static constexpr auto Message_Name{"message"};
		static constexpr auto Message_Default{""};
		static constexpr auto Message_Desc{"Message."};

	public:
		/// Default constructors
		Transfer();
		Transfer(Params&&);
		~Transfer() override;

		/// Provide the program name and a description.
		void init(const string& name, const string& desc) override;
		bool pass1(ParamPath&, ostream&) override;

		Transaction* root();
	private:
		/// menu command: tx

	protected: /// menu command: tx transfer

		ptr<Section> createSectionTxTransfer(); /// Init
		virtual bool main(Params&, ostream&); /// Command Handler
	
	private:
		ptr<core::Transfer> m_tx;
	};

}}}


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

#include "DigitalAssets.h"

namespace symbol { namespace core { namespace hmi {

	/// Human-Machine Interface. NFT section command processor (offline)
	class NonFungibleToken: public hmi::DigitalAssets {
		/// Base class b.
		using b = hmi::DigitalAssets;

	public:
		/// Flags, Options
		static constexpr const char* Info_Command = "info";
		static constexpr const char* Info_Command_Desc = "Non Fungible Token info.";

		static constexpr const char* NFT_Command = "nft";
		static constexpr const char* NFT_Command_Desc = "Functions related to N.F.T. (Non Fungible Tokens).";



	public:
		/// constructors, initialization, destructor
		using b::DigitalAssets;

		/// Provide the program name and a description.
		void init(const string& name, const string& desc) override;

	private:
		/// menu command: tx
		ptr<Section> createSectionNft();

	protected: /// menu command: tx transfer
		virtual ptr<Section> createSectionNftInfo(); /// Init
		virtual bool nftInfo(Params&, bool last, ostream&); /// Command Handler
	};
}}}


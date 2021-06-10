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

#include "Main.h"

namespace symbol { namespace core { namespace hmi {

	/// Human-Machine Interface. Keys section command processor (offline)
	class Keys: public hmi::Main {
		/// Base class b
		using b = hmi::Main;

		/// Flags and Options
		static constexpr char Privkey_Flag{'s'};
		static constexpr char Acc_Flag{'a'};

	public:
		/// Construction, initialization, destruction
		using b::Main;

		void init(const string& name, const string& desc) override;

	public:
		static FlagDef flagdefPrivkey(bool mandatory);
		static FlagDef flagdefAcc();

	public:
		/// Override to resolve a PublicKey from an Address.
		virtual ptr<PublicKey> resolvePublicKey(const UnresolvedAddress&) const { return nullptr; }

	private:
		/// menu command: keys
		ptr<Section> createSectionKeys(); /// init
		bool keys(const Params&, ostream&); /// Handler

	};

}}}


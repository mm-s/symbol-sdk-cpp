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
#include "base.h"
#include <iostream>
#include <map>
#include <vector>
#include "catapult/types.h"


namespace symbol { namespace core {

	using std::string;
	using std::pair;
	using std::ostream;
	using std::vector;

	/// Functions related to Digital Assets
	class DigitalAssets {
		/// Base class is b
	public:
		/// Construction, Initialization, Destruction
		DigitalAssets();
		
		using Hash = catapult::Hash160;
		
		static Hash digest(const vector<unsigned char>& buf);
		static ko readFile(const string& filename, vector<unsigned char>& buf);
 		static pair<ko, Hash> digestFile(const string& file);

	public:
		/// Serialization
		void toStream(ostream&) const;
	};

	using Hash = DigitalAssets::Hash;

}} // namespace symbol::core

std::ostream& operator << (std::ostream&, const symbol::core::DigitalAssets&);


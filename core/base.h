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
#include "config.h"
#include <string>
#include <iostream>
#include <sstream>

//#ifndef BOOST_ALL_DYN_LINK
//    error BOOST_ALL_DYN_LINK
//#endif

namespace symbol {
	using string = std::string;

	using ko_t = char;
	using ko = const ko_t*;
	static constexpr ko ok=nullptr;
	inline static bool is_ko(ko o) { return o!=ok; }
	inline static bool is_ok(ko o) { return o==ok; }

	template<class T> using ptr = T*;
	
} /// Namespace


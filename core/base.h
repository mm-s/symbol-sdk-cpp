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
#include <functional>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>

namespace symbol {
	using string = std::string;
	using ostream = std::ostream;
	using std::vector;
	using std::function;
	using std::exception;
	using std::move;
	using std::istringstream;
	using std::ostringstream;
	using std::pair;
	using std::map;
	using std::unordered_map;
	using std::make_pair;
	using std::cout;
	using std::cin;
	using std::endl;

	/// Error codes
	using ko_t = char;
	using ko = const ko_t*;
	static constexpr ko ok=nullptr;
	inline static bool is_ko(ko o) { return o!=ok; }
	inline static bool is_ok(ko o) { return o==ok; }

	/** Usage pattern for ko, ok, is_ko

		static constexpr auto KO_57763="Value is <0";

		ko foo(int i) { /// function returning error code
			if (i<0)
				return "Invalid";
			elseif (i==0) 
				return KO_57763;
			return ok;
		}

		ko bar(int i) {
			auto r=foo(i);
			if (is_ko(r) { ///check for error
				log(r);
				return r;
			}
			return ok;
		}
	**/

	/// Pointer type raw/smart
	template<class T> using ptr = T*;
	//using ptr = unique_ptr;

} /// Namespace


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
#include "Transfer.h"
#include "../../dto/dto.h"
#include <symbol/core/Hmi/Network.h>

namespace symbol { namespace hmi {

	using c = symbol::hmi::Transfer;
	using namespace std;

	Transaction* c::root() {
		return dynamic_cast<Transaction*>(b::root());
	}

	void c::pass1(ParamPath& v) {
		b::pass1(v);
		if (!root()->offline()) {
			auto p = v.lookup({"tx", "transfer"});
			if (p != nullptr) {
				p->set_optional(Mosaic_Flag);
				p->set_optional(Deadline_Flag);
				auto r = v.lookup({});
				assert(r != nullptr);
				r->set_optional(core::Hmi::Network::Seed_Flag);
			}
		}
	}

	bool c::txTransfer(Params&p, ostream& os) {
		if ( !root()->offline() ) {
			dto::node_info o;
			if ( !o.fetch( root()->url() ) ) {
				os << "Unable to fetch from " << root()->url() << ".";
				return false;
			}
			/// 
		}
		return b::txTransfer(p, os);
	}

}}


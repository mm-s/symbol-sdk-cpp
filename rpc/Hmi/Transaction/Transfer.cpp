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
#include <symbol/core/dto.h>
#include <symbol/core/Hmi/Network.h>
#include "../../Fetch.h"

namespace symbol { namespace hmi {

	using c = symbol::hmi::Transfer;
	using namespace std;

	Transaction* c::root() {
		return dynamic_cast<Transaction*>(b::root());
	}

	const Transaction* c::root() const {
		return dynamic_cast<const Transaction*>(b::root());
	}

	bool c::main(Params& p, bool last, ostream& os) {
		if (!b::main(p, last, os)) return false;
		if ( !root()->offline() ) {
			auto r=symbol::Fetch::ApiNode::info(root()->url());
			if (is_ko(r.first)) {
				os << r.first;
				return false;
			}
		}
		return true;
	}

	bool c::pass1(ParamPath& v, ostream& os) {
//cout << "RPC TRANSFER call base now" << endl;	
		if (!b::pass1(v, os)) return false;
//cout << "RPC TRANSFER" << endl;	
//cout << "pass1  " << root()->offline() << endl;
//cout << "A" << endl;
		auto p = v.lookup({});
		assert(p != nullptr);
		if (!p->is_set(Fetch::Offline_Flag)) {
			p->set_optional(core::Hmi::Network::Seed_Flag);
//cout << "C " << p->lookup(core::Hmi::Network::Seed_Flag)->name << " " << p->lookup(core::Hmi::Network::Seed_Flag)->optional << endl;
//cout << "Optional seed here because tx online" << endl;
//cout << "C " << p->lookup(core::Hmi::Network::Seed_Flag)->name << " " << p->lookup(core::Hmi::Network::Seed_Flag)->optional << endl;
			{
				auto p = v.lookup({Transaction::Main_Command, Transaction::Transfer_Command});
				if (p != nullptr) {
//	cout << "B" << endl;
					p->set_optional(Mosaic_Flag);
					p->set_optional(Deadline_Flag);
				}
			}
		}
		return true;
	}

}}


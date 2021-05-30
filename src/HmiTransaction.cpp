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
#include "HmiTransaction.h"

namespace symbol {

	using c = HmiTransaction;
	using namespace std;


	void c::rewrite(v_t& v) const {
		b::rewrite(v);
		auto p=v.lookup({"tx", "transfer"});
		if (p!=nullptr) {    
		    if (p->is_set(Grab_Flag)) {
		        p->set_optional(mosaic_flag);
		        p->set_optional(deadline_flag);
		        auto r=v.lookup({});
		        assert(r!=nullptr);
		        r->set_optional(Seed_Flag);
		    }
		}    
	}

	c::section* c::create_section_tx_transfer() {
		auto s=b::create_section_tx_transfer();
		s->add({Grab_Flag, "grab", true, false, "", "Obtain params from API node."});
		return s;
	}

	bool c::tx_transfer(const Params&p, ostream& os) {
		if (p.is_set(Grab_Flag)) {
		    //p.set_optional(mosaic_flag);
		    dto::node_info o;
		    if (!o.fetch(getUrl())) {
		        os << "Unable to fetch from " << getUrl() << ".";
		        return false;
		    }
		}
		return b::tx_transfer(p, os);
	}

}

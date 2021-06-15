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
#include "Network.h"
#include "Transaction.h"
#include "../catapult/LoggingConfiguration.h"

namespace symbol { namespace core { namespace hmi {
	
	using c = hmi::Network;

	c::Network(): b(Params{
			flagdefNetwork(),
			flagdefSeed(),
		}) {
	}

	c::Network(Params&&p): b(move(p)) {
		add(flagdefNetwork());
		add(flagdefSeed());
	}

	c::~Network() {
		delete m_network;
	}

	void c::init(const string& nm, const string& dc) {
		b::init(nm, dc);
		set_handler([&](const Params& p, ostream& os) -> bool { return mainHandler(p, os); });
	}

	c::FlagDef c::flagdefNetwork() {
		ostringstream os;
		os << "Network type. Value in (";
		core::Network::list(os);
		os << ").";
		return FlagDef{Network_Flag, "network", true, true, "public-test", os.str()};
	}

	c::FlagDef c::flagdefSeed() {
		return FlagDef{Seed_Flag, "seed", true, true, "", "Network generation hash seed."};
	}

	bool c::mainHandler(const Params& p, ostream& os) {
		m_networkOverriden = p.is_set(Network_Flag);
		core::Network::Identifier t = core::Network::identifier(p.get(Network_Flag));
		delete m_network;
		m_network = new core::Network(t);
		if (!network().isValidIdentifier()) {
			os << "Network identifier '" << p.get(Network_Flag) << "' is invalid.";
			return false;
		}
		
		if (p.is_set(Seed_Flag)) {
			if (!network().setSeed(p.get(Seed_Flag))) {
				os << "Network seed '" << p.get(Seed_Flag) << "' is invalid.";
				return false;
			}
		}
		return true;
	}

	void c::pass1(ParamPath& v) {
		b::pass1(v);
		//"tx", "transfer"  when the user runs this sequence reconfigure flag definitions (e.g. making some of them required)
		auto p=v.lookup({Transaction::TX_Command, Transaction::Transfer_Command}); //TODO replace strings with their section name var
		if (p!=nullptr) {
			auto r=v.lookup({});
			assert(r->has(Seed_Flag));
			r->set_mandatory(Seed_Flag);
		}
	}

}}}


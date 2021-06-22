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

	c::Params c::defParams() {
		return Params{
			{Network_Flag, Network_Name, true, true, Network_Default, Network_Desc},
			{Seed_Flag, Seed_Name, true, true, Seed_Default, Seed_Desc},
		};
	}

	c::Network(): b(defParams()) {
	}

	c::Network(Params&&p): b(move(defParams())) {
		add(move(p));
	}

	c::~Network() {
		delete m_network;
	}

	void c::help_flag(const FlagDef& f, ostream& os) const {
		if (f.short_name == Network_Flag) {
			os << "Possible values for --" << f.name << '\n';
			core::Network::list("  ", os);
			return;
		}
		b::help_flag(f, os);
	}

	ko c::setNetworkIdentifier(Params& p, const core::Network::Identifier& newId) {
		assert(m_network!=nullptr);
		if (!core::Network::isValid(newId)) {
			return "KO 66091 Invalid Network Id.";
		}
		if (m_networkOverriden) {
			if (m_network->identifier()!=newId) {
				return "KO 66095 Network Id set twice with different values.";
			}
			return ok;
		}
		if (m_network->identifier() != newId) {
			delete m_network;
			m_network = new core::Network(newId);
		}
		m_networkOverriden = true;
		
		Params& pn=const_cast<Params&>(p);
		auto f=pn.lookup(Network_Flag);
		f->value=symbol::core::Network::identifierStr(newId);
		return ok;
	}

	bool c::main(Params& p, ostream& os) {
		if (!b::main(p, os)) return false;
		assert(!m_networkOverriden);
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

	void c::init(const string& nm, const string& dc) {
		b::init(nm, dc);
	}

}}}


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
#include "Keys.h"

namespace symbol { namespace core { namespace hmi {

	using c = hmi::Keys;  /// Implementation for class c 

	c::Params c::defParams() {
		return Params{
			flagdefPrivkey(false),
			{Acc_Flag, Acc_Name, true, true, Acc_Default, Acc_Desc},
		};
	}

	c::FlagDef c::flagdefPrivkey(bool mandatory) {
		return FlagDef{Privkey_Flag, Privkey_Name, !mandatory, true, Privkey_Default, Privkey_Desc};
	}

	bool c::keys(Params& p, ostream& os) {
		if( p.is_set(Privkey_Flag) && p.is_set(Acc_Flag) ) {
			os << "Multiple inputs.";
			return false;
		}
		//secret key
		ptr<PrivateKey> sk{nullptr};
		if( !p.is_set(Privkey_Flag) && !p.is_set(Acc_Flag) ) {
			sk = symbol::Keys::newPrivateKey();
		}
		if ( p.is_set(Privkey_Flag) ) {
			sk = symbol::Keys::createPrivateKey( p.get(Privkey_Flag) );
			if (sk == nullptr) {
				os << "Input is not a private key.\n";
				return false;
			}
		}
		//public key, address
		ptr<PublicKey> pk{nullptr};
		ptr<UnresolvedAddress> addr{nullptr};
		if (sk != nullptr) {
			pk = symbol::Keys::createPublicKey(*sk);
			addr = network().newAddress(*pk);
		}
		else {
			string e = network().parse(p.get(Acc_Flag), pk, addr, networkOverriden());
			if ( !e.empty() ) {
				os << e << '\n';
				return false;
			}
		}
		if ( pk == nullptr ) {
			pk = resolvePublicKey(*addr);
		}
		//print
		vector<pair<string, string>> v;
		v.emplace_back(make_pair("network_id", toString(network().identifierHex())));
		v.emplace_back(make_pair("network", toString(network().identifierStr())));
		v.emplace_back(make_pair("private_key", sk!=nullptr ? toString(*sk) : string("unknown") ));
		v.emplace_back(make_pair("public_key", pk!=nullptr ? toString(*pk) : string("unknown") ));
		v.emplace_back(make_pair("address", toString(*addr) ));
		v.emplace_back(make_pair("account", addr->formatAccount() ));
		json() ? kv_json(v, os) : kv_text(v, os);
		delete sk;
		delete pk;
		delete addr;
		return true;
	}

	ptr<c::Section> c::createSectionKeys() {
		auto s=new Section(defParams());
		s->set_handler([&](Params& p, ostream& os) -> bool { return keys(p, os); });
		return s;
	}

	void c::init(const string& name, const string& desc) {
		b::init(name, desc);
		add(CmdDef{Keys_Command, Keys_Command_Desc}, createSectionKeys());
	}

}}}


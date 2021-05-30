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
#include "HmiFetch.h"
#include <restc-cpp/restc-cpp.h>
#include "dto/dto.h"

namespace symbol {

	using c = HmiFetch;
	using namespace std;
	using namespace restc_cpp;

	//auto endpoint="http://nem.mm-studios.com:3000";
	c::HmiFetch(): b(Params { flagdef_url() }) {

	}

	void c::init(const string& name, const string& desc) {
		b::init(name, desc);
		add_section(CmdDef{"fetch", "Obtaing remote data from an API node."}, create_section_fetch());
	}

	c::FlagDef c::flagdef_url() {
		return FlagDef{url_flag, "url", true, true, Def_Url, "API node URL."};
	}

	bool c::fetch(const Params& p, ostream& os) {
		return true;
	}

	c::section* c::create_section_fetch() {
		auto s=new section(Params{});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return fetch(p, os); });

		s->add_section(CmdDef{"node", "Node information."}, create_section_fetch_node());
		s->add_section(CmdDef{"chain", "Chain information."}, create_section_fetch_chain());
		s->add_section(CmdDef{"nodes", "List of network nodes."}, create_section_fetch_nodes());
		s->add_section(CmdDef{"account", "Account Info."}, create_section_fetch_account());

		return s;
	}

	bool c::fetch_node(const Params& p, ostream& os) {
		return true;
	}

	c::section* c::create_section_fetch_nodes() {
		auto s=new section(Params{});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return fetch_nodes(p, os); });
		return s;
	}

	c::section* c::create_section_fetch_account() {
		auto s=new section(Params{
		    {acc_flag, "address", false, true, "", "Account/Address/Public key."},
		});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return fetch_account(p, os); });
		return s;
	}

	c::section* c::create_section_fetch_node() {
		auto s=new section(Params{});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return fetch_node(p, os); });
		s->add_section(CmdDef{"health", "Node health."}, create_section_fetch_node_health());
		s->add_section(CmdDef{"info", "Node info."}, create_section_fetch_node_info());
		s->add_section(CmdDef{"peers", "Peers."}, create_section_fetch_node_peers());
		return s;
	}

	c::section* c::create_section_fetch_node_health() {
		auto s=new section(Params{});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return fetch_node_health(p, os); });
		return s;
	}

	c::section* c::create_section_fetch_node_info() {
		auto s=new section(Params{});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return fetch_node_info(p, os); });
		return s;
	}

	c::section* c::create_section_fetch_node_peers() {
		auto s=new section(Params{});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return fetch_node_peers(p, os); });
		return s;
	}

	bool c::fetch_node_health(const Params& p, ostream& os) {
		dto::node_health o;
		auto r=o.fetch(url);
		if (!r) return false;
		if (json) {
		    o.to_json(os);
		}
		else {
		    o.dump(os);
		}
		return true;    
	}

	bool c::fetch_node_peers(const Params& p, ostream& os) {
		dto::node_peers o;
		auto r=o.fetch(url);
		if (!r) return false;
		if (json) {
		    o.to_json(os);
		}
		else {
		    if (!hide_labels) o.dump_fields(os);
		    o.dump(os);
		}
		return true;    
	}

	bool c::fetch_node_info(const Params& p, ostream& os) {
		dto::node_info o;
		auto r=o.fetch(url);
		if (!r) return false;
		if (json) {
		    o.to_json(os);
		}
		else {
		    o.dump_table(os);
		}
		return true;    
	}

	bool c::fetch_account(const Params& p, ostream& os) {
		//public key, address
		symbol::keys::pub* pk{nullptr};
		symbol::net::address* addr{nullptr};
		string e=get_net().parse(p.get(acc_flag), pk, addr, is_net_overriden());
		if (!e.empty()) {
		    os << e << '\n';
		    return false;
		}
		string a=addr->format_account(); //"NASYMBOLLK6FSL7GSEMQEAWN7VW55ZSZU25TBOA";
		delete pk;
		delete addr;    
		    
		dto::account_t o;
		auto r=o.fetch(url, a);
		if (!r) return false;
		if (json) {
		    o.to_json(os);
		}
		else {
		    o.dump(os);
		}
		return true;    
	}

	bool c::fetch_nodes(const Params& p, ostream& os) {
		auto url=get_net().nodes_url();
		if (url.empty()) {
		    os << "Unknown Url for network '" << get_net() << "'.";
		    return false;
		}
		dto::net_list o;
		auto r=o.fetch(url);
		if (!r) return false;
		if (json) {
		    o.to_json(os);
		}
		else {
		    o.dump(os);
		}
		return true;    
	}

	c::section* c::create_section_fetch_chain() {
		auto s=new section(Params{});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return fetch_chain(p, os); });
		return s;
	}


	void chain_info(Context& ctx) {    // Here we are in a co-routine, running in a worker-thread.
/*
		auto reply = ctx.Get(string(endpoint)+"/chain/info"); //http://jsonplaceholder.typicode.com/posts/1");
		auto json = reply->GetBodyAsString();
		cout << "Received data: " << json << endl;
*/
	}

	bool c::fetch_chain(const Params& p, ostream& os) {
		os << "XXXXXXXXXXXChain\n";
		return true;
	}

	bool c::main_handler(const Params& p, ostream& os) {
		url=p.get(url_flag);
		return b::main_handler(p, os);
	}

}


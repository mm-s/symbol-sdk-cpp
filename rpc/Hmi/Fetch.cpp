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
#include "Fetch.h"
#include <restc-cpp/restc-cpp.h>
#include "../dto/dto.h"
////http://explorer.testnet.symboldev.network/

namespace symbol { namespace hmi {
	using c = symbol::hmi::Fetch;
	using string = std::string;

	c::Params c::defParams() {
		return Params{
			{Url_Flag, Url_Name, true, true, Url_Default, Url_Desc},
			{Offline_Flag, Offline_Name, true, false, Offline_Default, Offline_Desc},
		};
	}

	c::Fetch(): b(defParams()) {
	}

	c::Fetch(Params&&p): b(move(p)) {
		add(defParams());
	}

//	void c::chainInfo(Context& ctx) {    // Here we are in a co-routine, running in a worker-thread.
/*
		auto reply = ctx.Get(string(endpoint)+"/chain/info"); //http://jsonplaceholder.typicode.com/posts/1");
		auto json = reply->GetBodyAsString();
		cout << "Received data: " << json << endl;
*/
//	}

	bool c::fetchAccount(const Params& p, ostream& os) {
		ptr<symbol::PublicKey> pk{nullptr};
		ptr<symbol::UnresolvedAddress> addr{nullptr};
		string e = network().parse(p.get(Acc_Flag), pk, addr, networkOverriden());
		if (!e.empty()) {
			os << e << '\n';
			return false;
		}
		auto a=addr->formatAccount();
		delete pk;
		delete addr;
		dto::account_t o;
		auto r=o.fetch(m_url, a);
		if (!r) return false;
		if (json()) {
			o.to_json(os);
		}
		else {
			o.dump(os);
		}
		return true;
	}

	ptr<c::Section> c::createSectionFetchAccount() {
		auto s=new Section(Params{
			{core::Hmi::Keys::Acc_Flag, core::Hmi::Keys::Acc_Name, false, true, core::Hmi::Keys::Acc_Default, core::Hmi::Keys::Acc_Desc},
		});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return fetchAccount(p, os); });
		return s;
	}

	bool c::fetchNodes(const Params& p, ostream& os) {
		auto url = network().nodesUrl();
		if (url.empty()) {
			os << "Unknown Url for network '" << network() << "'.";
			return false;
		}
		dto::net_list o;
		auto r = o.fetch(url);
		if (!r) return false;
		if (json()) {
			o.to_json(os);
		}
		else {
			o.dump(os);
		}
		return true;
	}

	ptr<c::Section> c::createSectionFetchNodes() {
		auto s=new Section(Params{});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return fetchNodes(p, os); });
		return s;
	}

	bool c::fetchChain(const Params& p, ostream& os) {
		os << "XXXXXXXXXXXChain\n";
		return true;
	}

	c::section* c::createSectionFetchChain() {
		auto s = new Section(Params{});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return fetchChain(p, os); });
		return s;
	}

	bool c::fetchNodePeers(const Params& p, ostream& os) {
		dto::node_peers o;
		auto r=o.fetch(m_url);
		if (!r) return false;
		if (json()) {
			o.to_json(os);
		}
		else {
			if (!hideLabels()) o.dump_fields(os);
			o.dump(os);
		}
		return true;
	}

	ptr<c::Section> c::createSectionFetchNodePeers() {
		auto s=new Section(Params{});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return fetchNodePeers(p, os); });
		return s;
	}

	bool c::fetchNodeInfo(const Params& p, ostream& os) {
		dto::node_info o;
		auto r=o.fetch(m_url);
		if (!r) return false;
		if (json()) {
			o.to_json(os);
		}
		else {
			o.dump_table(os);
		}
		return true;
	}
	
	ptr<c::Section> c::createSectionFetchNodeInfo() {
		auto s=new Section(Params{});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return fetchNodeInfo(p, os); });
		return s;
	}

	bool c::fetchNodeHealth(const Params& p, ostream& os) {
		dto::node_health o;
		auto r=o.fetch(m_url);
		if (!r) return false;
		if (json()) {
			o.to_json(os);
		}
		else {
			o.dump(os);
		}
		return true;
	}

	ptr<c::Section> c::createSectionFetchNodeHealth() {
		auto s=new Section(Params{});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return fetchNodeHealth(p, os); });
		return s;
	}

	bool c::fetchNode(const Params& p, ostream& os) {
		return true;
	}

	ptr<c::Section> c::createSectionFetchNode() {
		auto s=new Section(Params{});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return fetchNode(p, os); });
		s->add(CmdDef{"health", "Node health."}, createSectionFetchNodeHealth());
		s->add(CmdDef{"info", "Node info."}, createSectionFetchNodeInfo());
		s->add(CmdDef{"peers", "Peers."}, createSectionFetchNodePeers());
		return s;
	}

	bool c::fetch(const Params& p, ostream& os) {
		if (p.is_set(Offline_Flag)) {
			os << "Cannot access the network in offline mode.";
			return false;
		}
		return true;
	}

	ptr<c::Section> c::createSectionFetch() {
		auto s=new Section(Params{});
		s->set_handler([&](Params& p, ostream& os) -> bool { return fetch(p, os); });
		s->add(CmdDef{"node", "Node information."}, createSectionFetchNode());
		s->add(CmdDef{"chain", "Chain information."}, createSectionFetchChain());
		s->add(CmdDef{"nodes", "List of network nodes."}, createSectionFetchNodes());
		s->add(CmdDef{"account", "Account Info."}, createSectionFetchAccount());
		return s;
	}

	void c::init(const string& name, const string& desc) {
		b::init(name, desc);
		add(CmdDef{"fetch", "Obtaing remote data from an API node."}, createSectionFetch());
	}

	void c::pass1(ParamPath& v) {
		b::pass1(v);
		auto p = v.lookup({""});
		if (p != nullptr) {    
			if ( p->is_set(Offline_Flag) ) {
				m_offline = true;
			}
		}
		
	}

	bool c::mainHandler(Params& p, ostream& os) {
		m_url = p.get(Url_Flag);
		return b::mainHandler(p, os);
	}

}}


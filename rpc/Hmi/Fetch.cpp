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
#include <symbol/core/dto.h>
#include "../Fetch.h"
#include <random>
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

	c::Fetch(Params&&p): b(defParams()) {
		add(move(p));
	}

	void c::help_flag(const FlagDef& f, ostream& os) const {
		if (f.short_name == Url_Flag) {
			os << Url_Desc << '\n';
			os << "Input example:\n";
			os << "    http://nem.mm-studios.com:3000\n";
			os << "Special values:\n";
			os << "    auto                    Selects a random node.\n";
			return;
		}
		b::help_flag(f, os);
	}


//	void c::chainInfo(Context& ctx) {    // Here we are in a co-routine, running in a worker-thread.
/*
		auto reply = ctx.Get(string(endpoint)+"/chain/info"); //http://jsonplaceholder.typicode.com/posts/1");
		auto json = reply->GetBodyAsString();
		cout << "Received data: " << json << endl;
*/
//	}

	bool c::fetchAccount(Params& p, bool last, ostream& os) {
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
		auto r=symbol::Fetch::account(url(), a);
		if (is_ko(r.first)) {
			os << r.first;
			return false;
		}
		print(r.second, os);
		return true;
	}

	ptr<c::Section> c::createSectionFetchAccount() {
		auto s=new Section(Params{
			{core::Hmi::Keys::Acc_Flag, core::Hmi::Keys::Acc_Name, false, true, core::Hmi::Keys::Acc_Default, core::Hmi::Keys::Acc_Desc},
		});
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return fetchAccount(p, last, os); });
		return s;
	}


	bool c::fetchNodesUrl(Params& p, bool last, ostream& os) {
		auto url = network().nodesUrl();
		os << "Url for fetching nodes for the network '" << network() << "': " << url << '\n';
		return true;
	}

	pair<ko, string> c::selectApiNode() const {
		using namespace std;
		auto r = dtoNodes();
		if (is_ko(r.first)) {
			return make_pair(r.first, "");
		}
		static mt19937_64 rng{random_device{}()};
		const core::dto::NetNodes& n=r.second;
		if (n.empty()) {
			return make_pair("KO 50499 Empty node list.", "");
		}
		uniform_int_distribution<> d(0, n.size()-1);
		auto i=next(n.begin(), d(rng));
		return make_pair(ok, i->url());
	}

	pair<ko, core::dto::NetNodes> c::dtoNodes() const {
		auto url = network().nodesUrl();
		if (url.empty()) {
			return make_pair("KO 11927 Unknown Url for network node list.", core::dto::NetNodes());
		}
		return symbol::Fetch::netNodes(url);
	}

	bool c::fetchNodes(Params& p, bool last, ostream& os) {
		if (!last) return true;
		auto r = dtoNodes();
		if (is_ko(r.first)) {
			os << r.first;
			return false;
		}
		print(r.second, os);
		return true;
	}

	ptr<c::Section> c::createSectionFetchNodes() {
		auto s=new Section(Params{});
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return fetchNodes(p, last, os); });
		s->add(CmdDef{"url", "Shows nodes fetch url."}, createSectionFetchNodesUrl());
		return s;
	}

	bool c::fetchChain(Params& p, bool last, ostream& os) {
		os << "Not implemented.\n";
		return true;
	}

	c::section* c::createSectionFetchChain() {
		auto s = new Section(Params{});
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return fetchChain(p, last, os); });
		return s;
	}

	bool c::fetchNodePeers(Params& p, bool last, ostream& os) {
		auto r=symbol::Fetch::ApiNode::peers(url());
		if (is_ko(r.first)) {
			os << r.first;
			return false;
		}
		print(r.second, os);
		return true;
	}

	ptr<c::Section> c::createSectionFetchNodesUrl() {
		auto s=new Section(Params{});
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return fetchNodesUrl(p, last, os); });
		return s;
	}

	ptr<c::Section> c::createSectionFetchNodePeers() {
		auto s=new Section(Params{});
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return fetchNodePeers(p, last, os); });
		return s;
	}

	bool c::fetchNodeInfo(Params& p, bool last, ostream& os) {
		auto r=symbol::Fetch::ApiNode::info(url());
		if (is_ko(r.first)) {
			os << r.first;
			return false;
		}
		print(r.second, os);
		return true;
	}
	
	ptr<c::Section> c::createSectionFetchNodeInfo() {
		auto s=new Section(Params{});
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return fetchNodeInfo(p, last, os); });
		return s;
	}

	bool c::fetchNodeHealth(Params& p, bool last, ostream& os) {
		auto r=symbol::Fetch::ApiNode::health(url());
		if (is_ko(r.first)) {
			os << r.first;
			return false;
		}
		print(r.second, os);
		return true;
	}

	ptr<c::Section> c::createSectionFetchNodeHealth() {
		auto s=new Section(Params{});
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return fetchNodeHealth(p, last, os); });
		return s;
	}

	bool c::fetchNode(Params& p, bool last, ostream& os) {
		return true;
	}

	ptr<c::Section> c::createSectionFetchNode() {
		auto s=new Section(Params{});
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return fetchNode(p, last, os); });
		s->add(CmdDef{"health", "Node health."}, createSectionFetchNodeHealth());
		s->add(CmdDef{"info", "Node info."}, createSectionFetchNodeInfo());
		s->add(CmdDef{"peers", "Peers."}, createSectionFetchNodePeers());
		return s;
	}

	bool c::cmdMain(Params& p, bool last, ostream& os) {
		if (p.is_set(Offline_Flag)) {
			os << "Cannot access the network in offline mode.";
			return false;
		}
		return true;
	}

	bool c::cmdExplorer(Params& p, bool last, ostream& os) {
		auto s = network().nodesUrl();
		if ( !s.empty() ) {
			os << "Explorer for network " << network() << " is at url: " << s << '\n';
		}
		else {
			os << "Unknown Explorer url for network " << network() << '\n';
		}
		return true;
	}

	ptr<c::Section> c::createSectionMain() {
		auto s=new Section(Params{});
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return cmdMain(p, last, os); });
		s->add(CmdDef{"node", "Node information."}, createSectionFetchNode());
		s->add(CmdDef{"chain", "Chain information."}, createSectionFetchChain());
		s->add(CmdDef{"nodes", "List of network nodes."}, createSectionFetchNodes());
		s->add(CmdDef{"account", "Account Info."}, createSectionFetchAccount());
		return s;
	}

	const string& c::url() const {
		static string empty;
		if (m_url=="auto") {
			auto r=selectApiNode();
			if (is_ko(r.first)) {
				return empty;
			}
			const_cast<string&>(m_url) = r.second;
		}
		return m_url; 
	
	}

//	bool c::main(Params& p, ostream& os) {
//		return b::mainHandler(p, os);
//		return true;
//	}
	ptr<c::Section> c::createSectionExplorer() {
		auto s=new Section(Params{});
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return cmdExplorer(p, last, os); });
		return s;
	}

	void c::init(const string& name, const string& desc) {
		b::init(name, desc);
		add(CmdDef{Main_Command, Main_Command_Desc}, createSectionMain());
		//add(CmdDef{Explorer_Command, Explorer_Command_Desc}, createSectionExplorer());
	}

	bool c::pass1(ParamPath& v, ostream& os) {
		if (!b::pass1(v, os)) return false;
		auto p = v.lookup({});
		assert(p!=nullptr);
		if ( p->is_set(Offline_Flag) ) {
			m_offline = true;
		}
		m_url = p->get(Url_Flag);
		return true;
	}

}}


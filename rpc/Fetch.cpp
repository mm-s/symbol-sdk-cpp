#include "Fetch.h"
#include <sstream>
#include <string>
#include <restc-cpp/restc-cpp.h>

namespace symbol {

//using namespace symbol;
//using namespace std;
//using namespace restc_cpp;

namespace {
	template<typename t>
	ko ufetch(t&o, const string& url, function<string(const string&)> jsonTransform=[](const string& i) -> string { return i; }) {
		auto cli = restc_cpp::RestClient::Create();
		auto f=cli->ProcessWithPromiseT<t>([url, jsonTransform](restc_cpp::Context& ctx) {
			std::clog << "[Accessing content from: " << url << "]\n";
			t o;
			o.fromJson(jsonTransform(ctx.Get(url)->GetBodyAsString()));
			return move(o);
		});
		ko r=ok;
		try {
			o = move(f.get());
		}
		catch(const exception& ex) {
			r = "KO 91023 Unable to fetch content from remote URL.";
		}
		cli->CloseWhenReady(true);
		return r;
	}
}

pair<ko, core::dto::Account> Fetch::account(const string& url, const string& account) {
	static constexpr const char* uri{"/accounts"};
	core::dto::Account o;
	ostringstream os;
	os << url << uri << '/' << account;
	auto r=ufetch(o, os.str(), [](const string& i) -> string { return i; });
	return make_pair(r, move(o));
}

pair<ko, core::dto::NetNodes> Fetch::netNodes(const string& url) {
	core::dto::NetNodes o;
	auto r=ufetch(o, url, [](const string& i) -> string { 
		ostringstream os;
		os << "{\"" << core::dto::NetNodes::Json_Element << "\": " << i << '}';
		return os.str();
	});
	return make_pair(r, move(o));
}

pair<ko, core::dto::ApiNode::Info> Fetch::ApiNode::info(const string& url) {
	static constexpr const char* uri{"/node/info"};
	core::dto::ApiNode::Info o;
	ostringstream os;
	os << url << uri;
	return make_pair(ufetch(o, os.str()), o);
}

pair<ko, core::dto::ApiNode::Peers> Fetch::ApiNode::peers(const string& url) {
	static constexpr const char* uri{"/node/peers"};
	core::dto::ApiNode::Peers o;
	ostringstream os;
	os << url << uri;
	return make_pair(ufetch(o, os.str(), [](const string& i) -> string { return string("{\"peers\": ") + i + "}"; }), o);
}

pair<ko, core::dto::ApiNode::Health> Fetch::ApiNode::health(const string& url) {
	static constexpr const char* uri{"/node/health"};
	core::dto::ApiNode::Health o;
	ostringstream os;
	os << url << uri;
	return make_pair(ufetch(o, os.str()), o);
}

}


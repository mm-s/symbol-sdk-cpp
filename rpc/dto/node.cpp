#include "node.h"
#include "fetch.h"
#include <boost/fusion/adapted.hpp>
#include <sstream>
#include <string>

using namespace symbol::dto;
using namespace std;
using namespace restc_cpp;

void node_peer::dump_fields(ostream& os) {
	os << "version publicKey networkGenerationHashSeed roles port networkIdentifier host friendlyName ";
}

void node_peer::dump_table(ostream& os) const {
	os << "version " << version << '\n';
	os << "publicKey " << publicKey << '\n';
	os << "networkGenerationHashSeed " << networkGenerationHashSeed << '\n';
	os << "roles " << roles << '\n';
	os << "port " << port << '\n';
	os << "networkIdentifier " << +networkIdentifier << '\n';
	os << "host " << host << '\n';
	os << "friendlyName " << friendlyName << '\n';
}

void node_peer::dump_line(ostream& os) const {
	os << version << ' ';
	os << publicKey << ' ';
	os << networkGenerationHashSeed << ' ';
	os << roles << ' ';
	os << port << ' ';
	os << +networkIdentifier << ' ';
	os << host << ' ';
	os << friendlyName << ' ';
}

BOOST_FUSION_ADAPT_STRUCT(
	node_peer,
	(auto, version)
	(auto, publicKey)
	(auto, networkGenerationHashSeed)
	(auto, roles)
	(auto, port)
	(auto, networkIdentifier)
	(auto, host)
	(auto, friendlyName)
)

BOOST_FUSION_ADAPT_STRUCT(
	node_peers,
	(auto, peers)
)

void node_peers::dump_fields(ostream& os) {
	node_peer::dump_fields(os);
	os << '\n';
}

void node_peers::dump(ostream& os) const {
	for (auto&i:peers) {
		i.dump_line(os);
		os << '\n';
	}
}

BOOST_FUSION_ADAPT_STRUCT(
	node_info,
	(auto, version)
	(auto, publicKey)
	(auto, networkGenerationHashSeed)
	(auto, roles)
	(auto, port)
	(auto, networkIdentifier)
	(auto, host)
	(auto, friendlyName)
	(auto, nodePublicKey)
)

void node_info::dump_table(ostream& os) const {
	b::dump_table(os);
	os << "nodePublicKey " << nodePublicKey << '\n';
}

void node_info::dump_line(ostream& os) const {
	b::dump_line(os);
	os << nodePublicKey << ' ';
}


BOOST_FUSION_ADAPT_STRUCT(
	node_health::status_t,
	(auto, apiNode)
	(auto, db)
)

BOOST_FUSION_ADAPT_STRUCT(
	node_health,
	(auto, status)
)

void node_health::dump(ostream& os) const {
	status.dump(os);
}

void node_health::status_t::dump(ostream& os) const {
	os << "apiNode " << apiNode << '\n';
	os << "db " << db << '\n';
}

bool node_health::from_json(const string& json) {
	return SerializeFromJson(*this, json);
}

void node_health::to_json(ostream& os) const {
	SerializeToJson(*this, os);
	os << '\n';
}

bool node_health::status_t::from_json(const string& json) {
	return SerializeFromJson(*this, json);
}

void node_health::status_t::to_json(ostream& os) const {
	SerializeToJson(*this, os);
	os << '\n';
}

bool node_peer::from_json(const string& json) {
	return SerializeFromJson(*this, json);
}

void node_peer::to_json(ostream& os) const {
	SerializeToJson(*this, os);
	os << '\n';
}

bool node_peers::from_json(const string& json) {
	return SerializeFromJson(*this, json);
}

void node_peers::to_json(ostream& os) const {
	SerializeToJson(*this, os);
	os << '\n';
}

bool node_info::from_json(const string& json) {
	return SerializeFromJson(*this, json);
}

void node_info::to_json(ostream& os) const {
	SerializeToJson(*this, os);
	os << '\n';
}

bool node_info::fetch(const string& url) {
	return ufetch(*this, url+uri);
}

bool node_peers::fetch(const string& url) {
	return ufetch(*this, url+uri, [](const string& i) -> string { return string("{\"peers\": ") + i + "}"; });
}

bool node_health::fetch(const string& url) {
	return ufetch(*this, url+uri);
}


#include "ApiNode.h"
#include <sstream>
#include <string>
#include <rapidjson/reader.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <iostream>

namespace symbol { namespace core { namespace dto {

void ApiNode::Health::Status::dump(ostream& os) const {
	os << "apiNode " << apiNode << '\n';
	os << "db " << db << '\n';
}

ko ApiNode::Health::Status::fromJson(const rapidjson::Value& v) {
	readField(apiNode, v, "apiNode");
	readField(db, v, "db");
	return ok;
}

void ApiNode::Health::Status::toText(const string& name, bool compact, const string& indent, ostream& os) const {
	writeField("apiNode", apiNode, compact, indent, os);
	writeField("db", db, compact, indent, os);
}

void ApiNode::Health::Status::toJson(rapidjson::Value& v, rapidjson::Document::AllocatorType& ator) const {
	writeField("apiNode", apiNode, v, ator);
	writeField("db", db, v, ator);
}

void ApiNode::Health::dump(ostream& os) const {
	status.dump(os);
}

vector<uint8_t> ApiNode::Health::toBin() const {
	vector<uint8_t> v;
	return move(v);
}

ko ApiNode::Health::fromJson(const rapidjson::Value& v) {
	readField(status, v, "status");
	return ok;
}

void ApiNode::Health::toText(const string& name, bool compact, const string& indent, ostream& os) const {
	writeField("status", status, compact, indent, os);
}

void ApiNode::Health::toJson(rapidjson::Value& v, rapidjson::Document::AllocatorType& ator) const {
	writeField("status", status, v, ator);
}

ko ApiNode::Health::fromJson(const string& json) {
	using namespace rapidjson;
	using namespace std;
	Document doc;
	if (doc.Parse(json.c_str()).HasParseError()) {
		cerr << (unsigned)doc.GetErrorOffset() << " " << GetParseError_En(doc.GetParseError()) << endl;
		return "KO 50948 Parse error.";
	}
	if (doc.IsNull()) return "KO 11823 Invalid json.";
	return fromJson(doc);
}

void ApiNode::Health::toJson(bool pretty, ostream& os) const {
	using namespace rapidjson;
	Document doc;
	doc.SetObject();
	Document::AllocatorType& ator = doc.GetAllocator();
	toJson(doc, ator);
	os << toString(doc, pretty) << endl;
}

void ApiNode::Peer::dumpFields(ostream& os) {
	os << "version publicKey networkGenerationHashSeed roles port networkIdentifier host friendlyName ";
}

void ApiNode::Peer::dumpTable(ostream& os) const {
	os << "version " << version << '\n';
	os << "publicKey " << publicKey << '\n';
	os << "networkGenerationHashSeed " << networkGenerationHashSeed << '\n';
	os << "roles " << roles << '\n';
	os << "port " << port << '\n';
	os << "networkIdentifier " << +networkIdentifier << '\n';
	os << "host " << host << '\n';
	os << "friendlyName " << friendlyName << '\n';
}

void ApiNode::Peer::dumpLine(ostream& os) const {
	os << version << ' ';
	os << publicKey << ' ';
	os << networkGenerationHashSeed << ' ';
	os << roles << ' ';
	os << port << ' ';
	os << +networkIdentifier << ' ';
	os << host << ' ';
	os << friendlyName << ' ';
}

ko ApiNode::Peer::fromJson(const rapidjson::Value& v) {
	readField(version, v, "version");
	readField(publicKey, v, "publicKey");
	readField(networkGenerationHashSeed, v, "networkGenerationHashSeed");
	readField(roles, v, "roles");
	readField(port, v, "port");
	readField(networkIdentifier, v, "networkIdentifier");
	readField(host, v, "host");
	readField(friendlyName, v, "friendlyName");
	return ok;
}

void ApiNode::Peer::toJson(rapidjson::Value& v, rapidjson::Document::AllocatorType& ator) const {
	writeField("version", version, v, ator);
	writeField("publicKey", publicKey, v, ator);
	writeField("networkGenerationHashSeed", networkGenerationHashSeed, v, ator);
	writeField("roles", roles, v, ator);
	writeField("port", port, v, ator);
	writeField("networkIdentifier", networkIdentifier, v, ator);
	writeField("host", host, v, ator);
	writeField("friendlyName", friendlyName, v, ator);
}

void ApiNode::Peer::toText(const string& name, bool compact, const string& indent, ostream& os) const {
	writeField("version", version, compact, indent, os);
	writeField("publicKey", publicKey, compact, indent, os);
	writeField("networkGenerationHashSeed", networkGenerationHashSeed, compact, indent, os);
	writeField("roles", roles, compact, indent, os);
	writeField("port", port, compact, indent, os);
	writeField("networkIdentifier", networkIdentifier, compact, indent, os);
	writeField("host", host, compact, indent, os);
	writeField("friendlyName", friendlyName, compact, indent, os);
}

void ApiNode::Peers::dumpFields(ostream& os) {
	Peer::dumpFields(os);
	os << '\n';
}

void ApiNode::Peers::dump(ostream& os) const {
	for (auto& i:*this) {
		i.dumpLine(os);
		os << '\n';
	}
}

ko ApiNode::Peers::fromJson(const rapidjson::Value& v) {
	readField(static_cast<b&>(*this), v, Json_Element);
	return ok;
}

vector<uint8_t> ApiNode::Peers::toBin() const {
	vector<uint8_t> v;
	return move(v);
}

ko ApiNode::Peers::fromJson(const string& json) {
	using namespace rapidjson;
	using namespace std;
	Document doc;
	if (doc.Parse(json.c_str()).HasParseError()) {
		cerr << (unsigned)doc.GetErrorOffset() << " " << GetParseError_En(doc.GetParseError()) << endl;
		return "KO 50948 Parse error.";
	}
	if (doc.IsNull()) return "KO 11823 Invalid json.";
	return fromJson(doc);
}

void ApiNode::Peers::toText(const string& name, bool compact, const string& indent, ostream& os) const {
	writeField(Json_Element, static_cast<const b&>(*this), compact, indent, os);
}

void ApiNode::Peers::toJson(rapidjson::Value& v, rapidjson::Document::AllocatorType& ator) const {
	writeField(Json_Element, static_cast<const b&>(*this), v, ator);
}

void ApiNode::Peers::toJson(bool pretty, ostream& os) const {
	using namespace rapidjson;
	Document doc;
	doc.SetObject();
	Document::AllocatorType& ator = doc.GetAllocator();
	toJson(doc, ator);
	os << toString(doc, pretty) << endl;
}


void ApiNode::Info::dumpTable(ostream& os) const {
	b::dumpTable(os);
	os << "nodePublicKey " << nodePublicKey << '\n';
}

void ApiNode::Info::dumpLine(ostream& os) const {
	b::dumpLine(os);
	os << nodePublicKey << ' ';
}


ko ApiNode::Info::fromJson(const rapidjson::Value& v) {
	return b::fromJson(v);
	readField(nodePublicKey, v, "nodePublicKey");
}

void ApiNode::Info::toJson(rapidjson::Value& v, rapidjson::Document::AllocatorType& ator) const {
	b::toJson(v, ator);
	writeField("nodePublicKey", nodePublicKey, v, ator);
}

void ApiNode::Info::toText(const string& name, bool compact, const string& indent, ostream& os) const {
	b::toText("", compact, indent, os);
	writeField("nodePublicKey", nodePublicKey, compact, indent, os);
}

vector<uint8_t> ApiNode::Info::toBin() const {
	vector<uint8_t> v;
	return move(v);
}

ko ApiNode::Info::fromJson(const string& json) {
//cout << json << endl;
	using namespace rapidjson;
	using namespace std;
	Document doc;
	if (doc.Parse(json.c_str()).HasParseError()) {
		cerr << (unsigned)doc.GetErrorOffset() << " " << GetParseError_En(doc.GetParseError()) << endl;
		return "KO 50948 Parse error.";
	}
	if (doc.IsNull()) return "KO 11823 Invalid json.";
	return fromJson(doc);
}

void ApiNode::Info::toJson(bool pretty, ostream& os) const {
	using namespace rapidjson;
	Document doc;
	doc.SetObject();
	Document::AllocatorType& ator = doc.GetAllocator();
	toJson(doc, ator);
	os << toString(doc, pretty) << endl;
}

string ApiNode::Info::toJson(bool pretty) const {
	ostringstream os;
	toJson(pretty, os);
	return os.str();
}

}}}


#include "Keys.h"
#include <sstream>
#include <string>
#include <rapidjson/reader.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <iostream>

namespace symbol { namespace core { namespace dto {

void Keys::dumpFields(ostream& os) {
	os << "networkId network privateKey publicKey address account\n";
}

void Keys::dumpLine(ostream& os) const {
	os << networkId << ' ';
	os << network << ' ';
	os << privateKey << ' ';
	os << publicKey << ' ';
	os << address << ' ';
	os << account << ' ';
	os << '\n';
}

ko Keys::fromJson(const rapidjson::Value& v) {
	readField(networkId, v, "networkId");
	readField(network, v, "network");
	readField(privateKey, v, "privateKey");
	readField(publicKey, v, "publicKey");
	readField(address, v, "address");
	readField(account, v, "account");
	return ok;
}

void Keys::toJson(rapidjson::Value& v, rapidjson::Document::AllocatorType& ator) const {
	writeField("networkId", networkId, v, ator);
	writeField("network", network, v, ator);
	writeField("privateKey", privateKey, v, ator);
	writeField("publicKey", publicKey, v, ator);
	writeField("address", address, v, ator);
	writeField("account", account, v, ator);
}


void Keys::toText(bool compact, ostream& os) const {
	if (compact) {
		os << networkId << ' ';
		os << network << ' ';
		os << privateKey << ' ';
		os << publicKey << ' ';
		os << address << ' ';
		os << account << ' ';
	}
	else {
		os << "networkId " << networkId << '\n';
		os << "network " << network << '\n';
		os << "privateKey " << privateKey << '\n';
		os << "publicKey " << publicKey << '\n';
		os << "address " << address << '\n';
		os << "account " << account << '\n';
	}
}

ko Keys::fromJson(const string& json) {
	using namespace rapidjson;
	using namespace std;
	Document doc;
	if (doc.Parse(json.c_str()).HasParseError()) {
		cerr << (unsigned)doc.GetErrorOffset() << " " << GetParseError_En(doc.GetParseError()) << endl;
		return "KO 50948 Parse error.";
	}
	if (doc.IsNull()) return "KO 11823 Invalid json.";
	readField(*this, doc, Json_Element);
	return ok;
}


void Keys::toJson(bool compact, ostream& os) const {
	using namespace rapidjson;
	Document doc;
	doc.SetObject();
	Document::AllocatorType& ator = doc.GetAllocator();
	toJson(doc, ator);
	os << toString(doc, !compact) << '\n';
}

string Keys::toJson(bool compact) const {
	ostringstream os;
	toJson(compact, os);
	return os.str();
}

}}}


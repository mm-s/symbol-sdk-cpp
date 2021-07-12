#include "Account.h"
#include <sstream>
#include <string>
#include <rapidjson/reader.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <iostream>

namespace symbol { namespace core { namespace dto {

using AccountData = Account::AccountData;


///--------------SupplementalPublicKeys

ko AccountData::SupplementalPublicKeys::fromJson(const rapidjson::Value& v) {
	readField(static_cast<b&>(*this), v, "");
	return ok;
}

void AccountData::SupplementalPublicKeys::toJson(rapidjson::Value& v, rapidjson::Document::AllocatorType& ator) const {
	writeField("", static_cast<const b&>(*this), v, ator);
}

void AccountData::SupplementalPublicKeys::toText(const string& name, bool compact, const string& indent, ostream& os) const {
	writeField("", static_cast<const b&>(*this), compact, indent, os);
}

///--------------Mosaic

ko AccountData::Mosaic::fromJson(const rapidjson::Value& v) {
	readField(id, v, "id");
	readField(amount, v, "amount");
	return ok;
}

void AccountData::Mosaic::toJson(rapidjson::Value& v, rapidjson::Document::AllocatorType& ator) const {
	writeField("id", id, v, ator);
	writeField("amount", amount, v, ator);
}

void AccountData::Mosaic::toText(const string& name, bool compact, const string& indent, ostream& os) const {
	writeField("id", id, compact, indent, os);
	writeField("amount", amount, compact, indent, os);
}

///--------------AccountData

ko AccountData::fromJson(const rapidjson::Value& v) {
	readField(version, v, "version");
	readField(address, v, "address");
	readField(addressHeight, v, "addressHeight");
	readField(publicKey, v, "publicKey");
	readField(publicKeyHeight, v, "publicKeyHeight");
	readField(supplementalPublicKeys, v, "supplementalPublicKeys");
	readField(activityBuckets, v, "activityBuckets");
	readField(mosaics, v, "mosaics");
	readField(importance, v, "importance");
	readField(importanceHeight, v, "importanceHeight");
	return ok;
}

void AccountData::toJson(rapidjson::Value& v, rapidjson::Document::AllocatorType& ator) const {
	writeField("version", version, v, ator);
	writeField("address", address, v, ator);
	writeField("addressHeight", addressHeight, v, ator);
	writeField("publicKey", publicKey, v, ator);
	writeField("publicKeyHeight", publicKeyHeight, v, ator);
	writeField("supplementalPublicKeys", supplementalPublicKeys, v, ator);
	writeField("accountType", accountType, v, ator);
	writeField("activityBuckets", activityBuckets, v, ator);
	writeField("mosaics", mosaics, v, ator);
	writeField("importance", importance, v, ator);
	writeField("importanceHeight", importanceHeight, v, ator);
}


void AccountData::toText(const string& name, bool compact, const string& indent, ostream& os) const {
	writeField("version", version, compact, indent, os);
	writeField("address", address, compact, indent, os);
	writeField("addressHeight", addressHeight, compact, indent, os);
	writeField("publicKey", publicKey, compact, indent, os);
	writeField("publicKeyHeight", publicKeyHeight, compact, indent, os);
	writeField("supplementalPublicKeys", supplementalPublicKeys, compact, indent, os);
	writeField("accountType", accountType, compact, indent, os);
	writeField("activityBuckets", activityBuckets, compact, indent, os);
	writeField("mosaics", mosaics, compact, indent, os);
	writeField("importance", importance, compact, indent, os);
	writeField("importanceHeight", importanceHeight, compact, indent, os);
}

///-------------------------Account

ko Account::fromJson(const rapidjson::Value& v) {
	readField(account, v, AccountData::Json_Element);
	readField(id, v, "id");
	return ok;
}

void Account::toJson(rapidjson::Value& v, rapidjson::Document::AllocatorType& ator) const {
	writeField(AccountData::Json_Element, account, v, ator);
	writeField("id", id, v, ator);
}

void Account::toText(const string& name, bool compact, const string& indent, ostream& os) const {
	writeField(AccountData::Json_Element, account, compact, indent, os);
	writeField("id", id, compact, indent, os);
}

void Account::toJson(bool compact, ostream& os) const {
	using namespace rapidjson;
	Document doc;
	doc.SetObject();
	Document::AllocatorType& ator = doc.GetAllocator();
	toJson(doc, ator);
	os << toString(doc, !compact) << '\n';
}

string Account::toJson(bool compact) const {
	ostringstream os;
	toJson(compact, os);
	return os.str();
}

ko Account::fromJson(const string& json) {
	using namespace rapidjson;
	using namespace std;
	Document doc;
	if (doc.Parse(json.c_str()).HasParseError()) {
		cerr << (unsigned)doc.GetErrorOffset() << " " << GetParseError_En(doc.GetParseError()) << endl;
		return "KO 50948 Parse error.";
	}
	if (doc.IsNull()) return "KO 11823 Invalid json.";
	fromJson(doc);
	return ok;
}

size_t Account::size() const {
	return 1; //account.size()+binSize(id);
}

void Account::toBin(vector<uint8_t>& v, size_t i) const {
	v[i]=1;
}


vector<uint8_t> Account::toBin() const {
	vector<uint8_t> v;
	auto sz=size();
	v.resize(sz);
	toBin(v, 0);
	//v.push_back('X');
	return move(v);
}

}}}


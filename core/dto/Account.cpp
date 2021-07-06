#include "Account.h"
#include <sstream>
#include <string>
#include <rapidjson/reader.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <iostream>

namespace symbol { namespace core { namespace dto {

ko Account::Mosaic::fromJson(const rapidjson::Value& v) {
	readField(id, v, "id");
	readField(amount, v, "amount");
	return ok;
}

void Account::Mosaic::toJson(rapidjson::Value& v, rapidjson::Document::AllocatorType& ator) const {
	writeField("id", id, v, ator);
	writeField("amount", amount, v, ator);
}

void Account::Mosaic::dumpFields(ostream& os) {
	os << "id amount ";
}

void Account::Mosaic::dumpLine(ostream& os) const {
	os << id << ' ';
	os << amount << ' ';
}

void Account::Mosaic::toText(bool compact, ostream& os) const {
	if (compact) {
		os << id << ' ';
		os << amount << ' ';
	}
	else {
		os << "id " << id << '\n';
		os << "amount " << amount << '\n';
	}
}

void Account::dumpFields(ostream& os) {
	os << "version address addressHeight publicKey publicKeyHeight accountType supplementalPublicKeys activityBuckets ";
	os << "[ ";
	Mosaic::dumpFields(os);
	os << "] ";
	os << "importance importanceHeight id ";
	os << '\n';
}

void Account::dumpLine(ostream& os) const {
	os << version << ' ';
	os << address << ' ';
	os << addressHeight << ' ';
	os << publicKey << ' ';
	os << publicKeyHeight << ' ';
	os << accountType << ' ';
	os << supplementalPublicKeys.size() << ' ';
	for (auto&i:supplementalPublicKeys) os << i << ' ';
	os << activityBuckets.size() << ' ';
	for (auto&i:activityBuckets) os << i << ' ';
	os << mosaics.size() << ' ';
	for (auto&i:mosaics) i.dumpLine(os);
	os << importance << ' ';
	os << importanceHeight << ' ';
	os << id << ' ';
	os << '\n';
}

ko Account::fromJson(const rapidjson::Value& v) {
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
	readField(id, v, "id");
	return ok;
}

void Account::toJson(rapidjson::Value& v, rapidjson::Document::AllocatorType& ator) const {
	writeField("version", version, v, ator);
	writeField("address", address, v, ator);
	writeField("addressHeight", addressHeight, v, ator);
	writeField("publicKey", publicKey, v, ator);
	writeField("publicKeyHeight", publicKeyHeight, v, ator);
	writeField("supplementalPublicKeys", supplementalPublicKeys, v, ator);
	writeField("activityBuckets", activityBuckets, v, ator);
	writeField("mosaics", mosaics, v, ator);
	writeField("importance", importance, v, ator);
	writeField("importanceHeight", importanceHeight, v, ator);
	writeField("id", id, v, ator);
	//writeField(Json_Element, static_cast<const b&>(*this), v, ator);
}


void Account::toText(bool compact, ostream& os) const {
	if (compact) {
		os << version << ' ';
		os << address << ' ';
		os << addressHeight << ' ';
		os << publicKey << ' ';
		os << publicKeyHeight << ' ';
		dto::toText(compact, supplementalPublicKeys, os);
		dto::toText(compact, activityBuckets, os);
		dto::toText(compact, mosaics, os);
		os << importance << ' ';
		os << importanceHeight << ' ';
		os << id << ' ';
	}
	else {
		os << "version " << version << '\n';
		os << "address " << address << '\n';
		os << "addressHeight " << addressHeight << '\n';
		os << "publicKey " << publicKey << '\n';
		os << "publicKeyHeight " << publicKeyHeight << '\n';
		os << "supplementalPublicKeys " << supplementalPublicKeys.size() << '\n';
		dto::toText(compact, supplementalPublicKeys, os);
		os << "activityBuckets " << activityBuckets.size() << '\n';
		dto::toText(compact, activityBuckets, os);
		os << "mosaics " << mosaics.size() << '\n';
		dto::toText(compact, mosaics, os);
		os << "importance " << importance << '\n';
		os << "importanceHeight " << importanceHeight << '\n';
		os << "id " << id << '\n';
	}
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
	readField(*this, doc, Json_Element);
	return ok;
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

}}}


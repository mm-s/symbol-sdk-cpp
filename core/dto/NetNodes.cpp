#include "NetNodes.h"
#include <sstream>
#include <string>
#include <rapidjson/reader.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/stringbuffer.h>
#include <iostream>

namespace symbol { namespace core { namespace dto {
	using namespace rapidjson;

void NetNode::PeerStatus::dumpLine(ostream& os) const {
	os << isAvailable << ' ';
	os << lastStatusCheck << ' ';
}

void NetNode::PeerStatus::dumpFields(ostream& os) {
	os << "isAvailable lastStatusCheck ";
}

ko NetNode::PeerStatus::fromJson(const rapidjson::Value& v) {
	readField(isAvailable, v, "isAvailable");
	readField(lastStatusCheck, v, "lastStatusCheck");
	return ok;
}

void NetNode::PeerStatus::toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) const {
	writeField("isAvailable", isAvailable, parent, ator);
	writeField("lastStatusCheck", lastStatusCheck, parent, ator);
}

void NetNode::ApiStatus::dumpLine(ostream& os) const {
	os << isAvailable << ' ';
	os << chainHeight << ' ';
	os << finalizationHeight << ' ';
	os << nodePublicKey << ' ';
	os << lastStatusCheck << ' ';
}

ko NetNode::ApiStatus::fromJson(const rapidjson::Value& v) {
	readField(isAvailable, v, "isAvailable");
	readField(chainHeight, v, "chainHeight");
	readField(finalizationHeight, v, "finalizationHeight");
	readField(nodePublicKey, v, "nodePublicKey");
	readField(lastStatusCheck, v, "lastStatusCheck");
	return ok;
}

void NetNode::ApiStatus::toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) const {
	writeField("isAvailable", isAvailable, parent, ator);
	writeField("chainHeight", chainHeight, parent, ator);
	writeField("finalizationHeight", finalizationHeight, parent, ator);
	writeField("nodePublicKey", nodePublicKey, parent, ator);
	writeField("finalizationHeight", finalizationHeight, parent, ator);
	writeField("lastStatusCheck", lastStatusCheck, parent, ator);
}


void NetNode::ApiStatus::dumpFields(ostream& os) {
	os << "isAvailable chainHeight finalizationHeight nodePublicKey lastStatusCheck ";
}

void NetNode::HostDetail::Coordinates::toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) const {
	writeField("latitude", latitude, parent, ator);
	writeField("longitude", longitude, parent, ator);
}

ko NetNode::HostDetail::Coordinates::fromJson(const rapidjson::Value& v) {
	readField(latitude, v, "latitude");
	readField(longitude, v, "longitude");
	return ok;
}

void NetNode::HostDetail::dumpLine(ostream& os) const {
	os << host << ' ';
	os << coordinates.latitude << ' ';
	os << coordinates.longitude << ' ';
	os << '"' << location << '"' << ' ';
	os << '"' << ip << '"' << ' ';
	os << '"' << organization << '"' << ' ';
	os << '"' << as << '"' << ' ';
	os << '"' << continent << '"' << ' ';
	os << '"' << country << '"' << ' ';
	os << '"' << region << '"' << ' ';
	os << '"' << city << '"' << ' ';
	os << '"' << district << '"' << ' ';
	os << '"' << zip << '"' << ' ';
}

void NetNode::HostDetail::dumpFields(ostream& os) {
	os << "host latitude longitude location ip organization as continent country region city district zip ";
}

void NetNode::HostDetail::toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) const {
	writeField("host", host, parent, ator);
	writeField(Coordinates::Json_Element, coordinates, parent, ator);
	writeField("location", location, parent, ator);
	writeField("ip", ip, parent, ator);
	writeField("organization", organization, parent, ator);
	writeField("as", as, parent, ator);
	writeField("continent", continent, parent, ator);
	writeField("country", country, parent, ator);
	writeField("region", region, parent, ator);
	writeField("city", city, parent, ator);
	writeField("region", region, parent, ator);
	writeField("district", district, parent, ator);
	writeField("zip", zip, parent, ator);
}

ko NetNode::HostDetail::fromJson(const rapidjson::Value& v) {
	readField(host, v, "host");
	readField(coordinates, v, Coordinates::Json_Element);
	readField(location, v, "location");
	readField(ip, v, "ip");
	readField(organization, v, "organization");
	readField(as, v, "as");
	readField(continent, v, "continent");
	readField(country, v, "country");
	readField(region, v, "region");
	readField(city, v, "city");
	readField(region, v, "region");
	readField(district, v, "district");
	readField(zip, v, "zip");
	return ok;
}

void NetNode::RewardProgram::dumpFields(ostream& os) {
	os << "id name passed";
}

void NetNode::RewardProgram::dumpLine(ostream& os) const {
	os << _id << ' ';
	os << name << ' ';
	os << passed << ' ';
}

void NetNode::RewardProgram::toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) const {
	writeField("_id", _id, parent, ator);
	writeField("name", name, parent, ator);
	writeField("passed", passed, parent, ator);
}

ko NetNode::RewardProgram::fromJson(const rapidjson::Value& v) {
	readField(_id, v, "_id");
	readField(name, v, "name");
	readField(passed, v, "passed");
	return ok;
}

void NetNode::dumpFields(ostream& os) {
	PeerStatus::dumpFields(os);
	ApiStatus::dumpFields(os);
	os << "_id version publicKey networkGenerationHashSeed roles port networkIdentifier host friendlyName rewardPrograms{";
	RewardProgram::dumpFields(os);
	os << "} ";
	HostDetail::dumpFields(os);
	os << " __v ";
}

void NetNode::dumpLine(ostream& os) const {
	peerStatus.dumpLine(os);
	apiStatus.dumpLine(os);
	os << _id << ' ';
	os << version << ' ';
	os << publicKey << ' ';
	os << networkGenerationHashSeed << ' ';
	os << roles << ' ';
	os << port << ' ';
	os << +networkIdentifier << ' ';
	os << host << ' ';
	os << friendlyName << ' ';
	os << rewardPrograms.size() << ' ';
	os << '{';
	for (auto&i:rewardPrograms) { i.dumpLine(os); }
	os << "} ";
	hostDetail.dumpLine(os);
	os << __v << ' ';
}

ko NetNode::fromJson(const rapidjson::Value& v) {
	readField(peerStatus, v, PeerStatus::Json_Element);
	readField(apiStatus, v, ApiStatus::Json_Element);
	readField(_id, v, "_id");
	readField(version, v, "version");
	readField(publicKey, v, "publicKey");
	readField(networkGenerationHashSeed, v, "networkGenerationHashSeed");
	readField(roles, v, "roles");
	readField(port, v, "port");
	readField(networkIdentifier, v, "networkIdentifier");
	readField(host, v, "host");
	readField(friendlyName, v, "friendlyName");
	readField(rewardPrograms, v, "rewardPrograms");
/*	
	auto& rp=v["rewardPrograms"];
	if (!rp.IsNull()) {
		rewardPrograms.clear();
		rewardPrograms.reserve(rp.Size());
		for (rapidjson::Value::ConstValueIterator itr = rp.Begin(); itr != rp.End(); ++itr) {
			RewardProgram o;
			o.fromJson(*itr);
			rewardPrograms.push_back(o);
		}
	}
*/
	readField(hostDetail, v, HostDetail::Json_Element);
	readField(__v, v, "__v");
	return ok;
}

void NetNode::toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) const {
	writeField(PeerStatus::Json_Element, peerStatus, parent, ator);
	writeField(ApiStatus::Json_Element, apiStatus, parent, ator);
	writeField("_id", _id, parent, ator);
	writeField("version", version, parent, ator);
	writeField("publicKey", publicKey, parent, ator);
	writeField("networkGenerationHashSeed", networkGenerationHashSeed, parent, ator);
	writeField("roles", roles, parent, ator);
	writeField("port", roles, parent, ator);
	writeField("networkIdentifier", networkIdentifier, parent, ator);
	writeField("host", host, parent, ator);
	writeField("friendlyName", friendlyName, parent, ator);
	writeField("rewardPrograms", rewardPrograms, parent, ator);
	writeField("hostDetail", hostDetail, parent, ator);
	writeField("__v", __v, parent, ator);


/*
	readField(peerStatus, v, PeerStatus::Json_Element);
	readField(apiStatus, v, ApiStatus::Json_Element);
	readField(_id, v, "_id");
	readField(version, v, "version");
	readField(publicKey, v, "publicKey");
	readField(networkGenerationHashSeed, v, "networkGenerationHashSeed");
	readField(roles, v, "roles");
	readField(port, v, "port");
	readField(networkIdentifier, v, "networkIdentifier");
	readField(host, v, "host");
	readField(friendlyName, v, "friendlyName");
	auto& rp=v["rewardPrograms"];
	if (!rp.IsNull()) {
		rewardPrograms.clear();
		rewardPrograms.reserve(rp.Size());
		for (rapidjson::Value::ConstValueIterator itr = rp.Begin(); itr != rp.End(); ++itr) {
			RewardProgram o;
			o.fromJson(*itr);
			rewardPrograms.push_back(o);
		}
	}
	readField(hostDetail, v, HostDetail::Json_Element);
	readField(__v, v, "__v");
*/
//	Value v;
//	v.SetString(StringRef(_id.c_str()));
//	parent.AddMember("_id", v, ator);
}


void NetNodes::dumpFields(ostream& os) {
	NetNode::dumpFields(os);
	os << '\n';
}

void NetNodes::dump(ostream& os) const {
	for (auto&i:*this) {
		i.dumpLine(os);
		os << '\n';
	}
}

ko NetNodes::fromJson(const rapidjson::Value& v) {
	readField(static_cast<b&>(*this), v, Json_Element);
/*
	reserve(v.Size());
	for (rapidjson::Value::ConstValueIterator itr = v.Begin(); itr != v.End(); ++itr) { // Ok
		NetNode o;
		auto r=o.fromJson(*itr);
		if (is_ko(r)) {
			return r;
		}
		push_back(o);
	}
*/
	return ok;
}

void NetNodes::toJson(rapidjson::Value& v, rapidjson::Document::AllocatorType& ator) const {
	writeField(Json_Element, static_cast<const b&>(*this), v, ator);
}

ko NetNodes::fromJson(const string& json) {
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

void NetNodes::toJson(bool pretty, ostream& os) const {
	using namespace rapidjson;
	Document doc;
	doc.SetObject();
	Document::AllocatorType& ator = doc.GetAllocator();
	toJson(doc, ator);
	os << toString(doc, pretty) << endl;
}

string NetNodes::toJson(bool pretty) const {
	ostringstream os;
	toJson(pretty, os);
	return os.str();
}

void NetNodes::toText(bool compact, ostream& os) const {
}

vector<uint8_t> NetNodes::toBin() const {
	vector<uint8_t> v;
	return move(v);
}

}}}


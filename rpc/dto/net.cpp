#include "net.h"
#include "fetch.h"
#include <sstream>
#include <string>
#include <rapidjson/reader.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <iostream>

using namespace symbol::dto;
using namespace std;
using namespace restc_cpp;

namespace {
	using namespace rapidjson;
	using namespace std;

	struct MyHandler : public BaseReaderHandler<UTF8<>, MyHandler> {
		bool Null() { cout << "Null()" << endl; return true; }
		bool Bool(bool b) { cout << "Bool(" << boolalpha << b << ")" << endl; return true; }
		bool Int(int i) { cout << "Int(" << i << ")" << endl; return true; }
		bool Uint(unsigned u) { cout << "Uint(" << u << ")" << endl; return true; }
		bool Int64(int64_t i) { cout << "Int64(" << i << ")" << endl; return true; }
		bool Uint64(uint64_t u) { cout << "Uint64(" << u << ")" << endl; return true; }
		bool Double(double d) { cout << "Double(" << d << ")" << endl; return true; }
		bool String(const char* str, SizeType length, bool copy) { 
			cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
			return true;
		}
		bool StartObject() {
			 cout << "StartObject()" << endl;
			 return true; 
		}
		bool Key(const char* str, SizeType length, bool copy) { 
			
			cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
			return true;
		}
		bool EndObject(SizeType memberCount) { cout << "EndObject(" << memberCount << ")" << endl; return true; }
		bool StartArray() { cout << "StartArray()" << endl; return true; }
		bool EndArray(SizeType elementCount) { cout << "EndArray(" << elementCount << ")" << endl; return true; }
	};
}

void net_node::peerStatus_t::dump_line(ostream& os) const {
	os << isAvailable << ' ';
	os << lastStatusCheck << ' ';
}

void net_node::peerStatus_t::dump_fields(ostream& os) {
	os << "isAvailable lastStatusCheck ";
}

namespace {
	void print(const rapidjson::Value& v) {
		using namespace rapidjson;
		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		v.Accept(writer);

		// Output {"project":"rapidjson","stars":11}
		cout << buffer.GetString() << endl;
	}
}


bool net_node::peerStatus_t::from_json(const rapidjson::Value& v) {
	isAvailable=false;
	lastStatusCheck=0;
	if (v.IsNull()) return true;
	isAvailable = v["isAvailable"].GetBool(); // error
	lastStatusCheck = v["lastStatusCheck"].GetUint64(); // error
	return true;
}

void net_node::apiStatus_t::dump_line(ostream& os) const {
	os << isAvailable << ' ';
	os << chainHeight << ' ';
	os << finalizationHeight << ' ';
	os << nodePublicKey << ' ';
	os << lastStatusCheck << ' ';
}

bool net_node::apiStatus_t::from_json(const rapidjson::Value& v) {
	isAvailable=false;
	chainHeight=0;
	finalizationHeight=0;
	nodePublicKey="";
	lastStatusCheck=0;
	if (v.IsNull()) return true;
	isAvailable = v["isAvailable"].GetBool();
	if (isAvailable) {
		chainHeight=v["chainHeight"].GetUint64();
		finalizationHeight=v["finalizationHeight"].GetUint64();
		nodePublicKey=v["nodePublicKey"].GetString();
		lastStatusCheck = v["lastStatusCheck"].GetUint64();
	}
	return true;
}

void net_node::apiStatus_t::dump_fields(ostream& os) {
	os << "isAvailable chainHeight finalizationHeight nodePublicKey lastStatusCheck ";
}

bool net_node::hostDetail_t::coordinates_t::from_json(const rapidjson::Value& v) {
	latitude=0;
	longitude=0;
	if (v.IsNull()) return true;
	latitude=v["latitude"].GetDouble();
	longitude=v["longitude"].GetDouble();
	return true;
}

void net_node::hostDetail_t::dump_line(ostream& os) const {
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

void net_node::hostDetail_t::dump_fields(ostream& os) {
	os << "host latitude longitude location ip organization as continent country region city district zip ";
}

bool net_node::hostDetail_t::from_json(const rapidjson::Value& v) {
	host="";
	location="";
	ip="";
	organization="";
	as="";
	continent="";
	country="";
	region="";
	city="";
	district="";
	zip="";
	if (v.IsNull()) return true;
//print(v);
	host = v["host"].GetString();
	coordinates.from_json(v[coordinates_t::Json_Element]);
	location=v["location"].GetString();
	ip=v["ip"].GetString();
	organization=v["organization"].GetString();
	as=v["as"].GetString();
	continent=v["continent"].GetString();
	country=v["country"].GetString();
	region=v["region"].GetString();
	city=v["city"].GetString();
	district=v["district"].GetString();
	zip=v["zip"].GetString();
	return true;
}

void net_node::RewardProgram::dump_fields(ostream& os) {
	os << "id name passed";
}

void net_node::RewardProgram::dump_line(ostream& os) const {
	os << _id << ' ';
	os << name << ' ';
	os << passed << ' ';
}

bool net_node::RewardProgram::from_json(const rapidjson::Value& v) {
	_id="";
	name="";
	passed=false;
	if (v.IsNull()) return true;
	_id = v["_id"].GetString();
	name=v["name"].GetString();
	passed=v["passed"].GetBool();
	return true;
}

void net_node::dump_fields(ostream& os) {
	peerStatus_t::dump_fields(os);
	apiStatus_t::dump_fields(os);
	os << "_id version publicKey networkGenerationHashSeed roles port networkIdentifier host friendlyName rewardPrograms{";
	RewardProgram::dump_fields(os);
	os << "} ";
	hostDetail_t::dump_fields(os);
	os << " __v ";
}

void net_node::dump_line(ostream& os) const {
	peerStatus.dump_line(os);
	apiStatus.dump_line(os);
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
	for (auto&i:rewardPrograms) { i.dump_line(os); }
	os << "} ";
	hostDetail.dump_line(os);
	os << __v << ' ';
}

bool net_node::from_json(const string& json) {
	return SerializeFromJson(*this, json);
}


bool net_node::from_json(const rapidjson::Value& v) {
	_id="";
	version=0;
	publicKey="";
	networkGenerationHashSeed="";
	roles=0;
	port=0;
	networkIdentifier=0;
	host="";
	friendlyName="";
	__v=0;
	if (v.IsNull()) return true;

	peerStatus.from_json(v[peerStatus_t::Json_Element]);
	apiStatus.from_json(v[apiStatus_t::Json_Element]);
	_id = v["_id"].GetString();
	version=v["version"].GetInt();
	publicKey=v["publicKey"].GetString();
	networkGenerationHashSeed=v["networkGenerationHashSeed"].GetString();
	roles = v["roles"].GetInt();
	port = v["port"].GetInt();
	networkIdentifier = v["networkIdentifier"].GetInt();
	host = v["host"].GetString();
	friendlyName = v["friendlyName"].GetString();
	auto& rp=v["rewardPrograms"];
	if (!rp.IsNull()) {
		rewardPrograms.clear();
		rewardPrograms.reserve(rp.Size());
		for (rapidjson::Value::ConstValueIterator itr = rp.Begin(); itr != rp.End(); ++itr) {
			RewardProgram o;
			o.from_json(*itr);
			rewardPrograms.push_back(o);
		}
	}
	hostDetail.from_json(v[hostDetail_t::Json_Element]);
	__v=v["__v"].GetInt();
/*
	if (v.GetString()==string(net_node::)) {
		
	}
	else {
		cout << "unhandled: " << v.GetString() << endl;
	}
*/
	return true;
}


void net_node::to_json(ostream& os) const {
	SerializeToJson(*this, os);
	os << '\n';
}

void nodes::dump_fields(ostream& os) {
	net_node::dump_fields(os);
	os << '\n';
}

void nodes::dump(ostream& os) const {
	for (auto&i:*this) {
		i.dump_line(os);
		os << '\n';
	}
}

bool nodes::from_json(const rapidjson::Value& v) {
	reserve(v.Size());
	for (rapidjson::Value::ConstValueIterator itr = v.Begin(); itr != v.End(); ++itr) { // Ok
		net_node o;
		if (!o.from_json(*itr)) {
			return false;
		}
		push_back(o);
	}
	return true;
}

bool nodes::from_json(const string& json) {
	using namespace rapidjson;
	using namespace std;
/*
	MyHandler handler;
	Reader reader;
	StringStream ss(json.c_str());
	reader.Parse(ss, handler);
*/
//cout << json << endl;
	Document doc;
	if (doc.Parse(json.c_str()).HasParseError()) {
		cerr << (unsigned)doc.GetErrorOffset() << " " << GetParseError_En(doc.GetParseError()) << endl;
		return false;
	}
	if(doc.GetString()!=string(Json_Element)) { //"nodes"
		return false;
	}
	from_json(doc[Json_Element]);
	return true;
}

void nodes::to_json(ostream& os) const {
	SerializeToJson(*this, os);
	os << '\n';
}

bool nodes::fetch(const string& url) {
	return ufetch(*this, url, [](const string& i) -> string { 
		ostringstream os;
		os << "{\"" << Json_Element << "\": " << i << '}';
		return os.str();
	 });
}



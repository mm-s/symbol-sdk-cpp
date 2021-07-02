#include "net.h"
#include "fetch.h"
#include <boost/fusion/adapted.hpp>
#include <sstream>
#include <string>
#include <rapidjson/reader.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include <iostream>

using namespace symbol::dto;
using namespace std;
using namespace restc_cpp;

BOOST_FUSION_ADAPT_STRUCT(
	net_node::peerStatus_t,
	(auto, isAvailable)
	(auto, lastStatusCheck)
)

BOOST_FUSION_ADAPT_STRUCT(
	net_node::apiStatus_t,
	(auto, isAvailable)
	(auto, chainHeight)
	(auto, finalizationHeight)
	(auto, nodePublicKey)
	(auto, lastStatusCheck)
)

BOOST_FUSION_ADAPT_STRUCT(
	net_node::hostDetail_t::coordinates_t,
	(auto, latitude)
	(auto, longitude)
)

BOOST_FUSION_ADAPT_STRUCT(
	net_node::hostDetail_t,
	(auto, host)
	(auto, coordinates)
	(auto, location)
	(auto, ip)
	(auto, organization)
	(auto, as)
	(auto, continent)
	(auto, country)
	(auto, region)
	(auto, city)
	(auto, district)
	(auto, zip)
)

BOOST_FUSION_ADAPT_STRUCT(
	net_node::RewardProgram,
	(auto, _id)
	(auto, name)
	(auto, passed)
)

BOOST_FUSION_ADAPT_STRUCT(
	net_node,
	(auto, peerStatus)
	(auto, apiStatus)
	(auto, _id)
	(auto, version)
	(auto, publicKey)
	(auto, networkGenerationHashSeed)
	(auto, roles)
	(auto, port)
	(auto, networkIdentifier)
	(auto, host)
	(auto, friendlyName)
	(auto, rewardPrograms)
	(auto, hostDetail)
	(auto, __v)
)

BOOST_FUSION_ADAPT_STRUCT(
	net_list,
	(auto, nodes)
)

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

void net_node::apiStatus_t::dump_line(ostream& os) const {
	os << isAvailable << ' ';
	os << chainHeight << ' ';
	os << finalizationHeight << ' ';
	os << nodePublicKey << ' ';
	os << lastStatusCheck << ' ';
}

void net_node::apiStatus_t::dump_fields(ostream& os) {
	os << "isAvailable chainHeight finalizationHeight nodePublicKey lastStatusCheck ";
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

void net_node::RewardProgram::dump_fields(ostream& os) {
	os << "id name passed";
}

void net_node::RewardProgram::dump_line(ostream& os) const {
	os << _id << ' ';
	os << name << ' ';
	os << passed << ' ';
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

bool net_node::from_json(net_node& o, const string& json) {
	return SerializeFromJson(o, json);
}

void net_node::to_json(ostream& os) const {
	SerializeToJson(*this, os);
	os << '\n';
}

void net_list::dump_fields(ostream& os) {
	net_node::dump_fields(os);
	os << '\n';
}

void net_list::dump(ostream& os) const {
	for (auto&i:nodes) {
		i.dump_line(os);
		os << '\n';
	}
}


bool net_list::from_json(net_list& o, const string& json) {
	using namespace rapidjson;
	using namespace std;
/*
	MyHandler handler;
	Reader reader;
	StringStream ss(json.c_str());
	reader.Parse(ss, handler);
*/
Document d;
if (d.Parse(json.c_str()).HasParseError()) {
	cerr << (unsigned)d.GetErrorOffset() << " " << GetParseError_En(d.GetParseError()) << endl;
	return false;
}
	//o.from_json(d);
/*
	Json::Value root;
	Json::Reader reader;
	bool r = reader.parse( json, root );
	if ( !r ) {
		cout << "Error parsing the string" << endl;
	}

    const Json::Value mynames = root["people"];

    for ( int index = 0; index < mynames.size(); ++index )
    {
        cout << mynames[index] << endl;
    }
    
*/

//"apiStatus":{"isAvailable":false,"chainHeight":0,"finalizationHeight":0,"nodePublicKey":"","restVersion":"","lastStatusCheck":0},
/*
ostringstream os;
istringstream is(json);
while(is.good()) {
	string s;
	is >> s;
	os << s;
cout << s << endl;
}
//cout << os.str() << endl;
*/


//cout << json << endl;
//auto x="{\"nodes\": [{ \"peerStatus\": {\"isAvailable\":true,	\"lastStatusCheck\":1625087763198	}, \"apiStatus\":	{\"isAvailable\":true,\"chainHeight\":208154,\"finalizationHeight\":208136,	\"nodePublicKey\":\"A2160AB911943082C88109DD8B65A0082EF547CA7C28F001F857112F7ADD9B3D\",	\"restVersion\":\"2.3.6\",	\"lastStatusCheck\":1625087763252	}, \"_id\":\"60dcdf23872851001260ca5f\", \"version\":16777472, \"publicKey\":\"E3FC28889BDE31406465167F1D9D6A16DCA1FF67A3BABFA5E5A8596478848F78\", \"networkGenerationHashSeed\":\"3B5E1FA6445653C971A50687E75E6D09FB30481055E3990C84B25E9222DC1155\", \"roles\":3, \"port\":7900, \"networkIdentifier\":152, \"host\":\"ngl-dual-001.testnet.symboldev.network\", \"friendlyName\":\"ngl-dual-001\", \"rewardPrograms\":		[		{		\"_id\":\"60dcdf23872851001260ca60\",		\"name\":\"SuperNode\",		\"passed\":true		}		], \"hostDetail\":	{	\"host\":\"ngl-dual-001.testnet.symboldev.network\",	\"coordinates\":		{		\"latitude\":39.0438,			\"longitude\":-77.4874		},	\"location\":\"Ashburn, VA, United States\",	\"ip\":\"3.86.56.197\",	\"organization\":\"AWS EC2 (us-east-1)\",	\"as\":\"AS14618 Amazon.com, Inc.\",	\"continent\":\"North America\",	\"country\":\"United States\",	\"region\":\"VA\",	\"city\":\"Ashburn\",	\"district\":\"\",	\"zip\":\"20149\"	},	\"__v\":0 }]}";
//auto x="{\"nodes\": [{\"peerStatus\":{\"isAvailable\":true,\"lastStatusCheck\":1625094672083},\"apiStatus\":{\"isAvailable\":true,\"chainHeight\":305008,\"finalizationHeight\":304976,\"nodePublicKey\":\"AA827C4E0F809AD586629AD341928336DCD62E7E09F73806886143CF0FB1E958\",\"restVersion\":\"2.3.6\",\"lastStatusCheck\":1625094674793},\"_id\":\"60dcfa24ad707a001232a774\",\"version\":16777472,\"publicKey\":\"C7BEA9036ECFA79CB081184CFA0E524E7D567A5127C55360D9FF1D2FC1AC4FDD\",\"networkGenerationHashSeed\":\"57F7DA205008026C776CB6AED843393F04CD458E0AA2D9F1D5F31A402072B2D6\",\"roles\":7,\"port\":7900,\"networkIdentifier\":104,\"host\":\"ngl-dual-001.symbolblockchain.io\",\"friendlyName\":\"ngl-dual-001\",\"rewardPrograms\":[],\"hostDetail\":{\"host\":\"ngl-dual-001.symbolblockchain.io\",\"coordinates\":{\"latitude\":39.0438,\"longitude\":-77.4874},\"location\":\"Ashburn, VA, United States\",\"ip\":\"54.90.245.174\",\"organization\":\"AWS EC2 (us-east-1)\",\"as\":\"AS14618 Amazon.com, Inc.\",\"continent\":\"North America\",\"country\":\"United States\",\"region\":\"VA\",\"city\":\"Ashburn\",\"district\":\"\",\"zip\":\"20149\"},\"__v\":0}]}";
//cout << x << endl;
//	return SerializeFromJson(o, x);
	return SerializeFromJson(o, json);
}

void net_list::to_json(ostream& os) const {
	SerializeToJson(*this, os);
	os << '\n';
}

bool net_list::fetch(const string& url) {
	return ufetch(*this, url, [](const string& i) -> string { 
		ostringstream os;
		os << "{\"nodes\": " << i << '}';
		return os.str();
	 });
}



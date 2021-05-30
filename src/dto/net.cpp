#include "net.h"
#include "fetch.h"
#include <boost/fusion/adapted.hpp>
#include <sstream>
#include <string>

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

void net_node::dump_fields(ostream& os) {
    peerStatus_t::dump_fields(os);
    apiStatus_t::dump_fields(os);
    os << "_id version publicKey networkGenerationHashSeed roles port networkIdentifier host friendlyName [rewardPrograms] ";
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
    for (auto&i:rewardPrograms) os << i << ' ';
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
cout << json << endl;
    return SerializeFromJson(o, json);
}

void net_list::to_json(ostream& os) const {
    SerializeToJson(*this, os);
    os << '\n';
}

bool net_list::fetch(const string& url) {
    return ufetch(*this, url, [](const string& i) -> string { return string("{\"nodes\": ") + i + "}"; });
}
          
          
          
          

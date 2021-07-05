#include "account.h"
#include "fetch.h"
#include <boost/fusion/adapted.hpp>
#include <sstream>
#include <string>

using namespace symbol::dto;
using namespace std;
using namespace restc_cpp;

BOOST_FUSION_ADAPT_STRUCT(
	account_t::account_data_t::mosaic_t,
	(auto, id)
	(auto, amount)
)

BOOST_FUSION_ADAPT_STRUCT(
	account_t::account_data_t,
	(auto, version)
	(auto, address)
	(auto, addressHeight)
	(auto, publicKey)
	(auto, publicKeyHeight)
	(auto, accountType)
	(auto, supplementalPublicKeys)
	(auto, activityBuckets)
	(auto, mosaics)
	(auto, importance)
	(auto, importanceHeight)
	(auto, id)
)

BOOST_FUSION_ADAPT_STRUCT(
	account_t,
	(auto, account)
)

void account_t::account_data_t::mosaic_t::dump_fields(ostream& os) {
	os << "id amount ";
}

void account_t::account_data_t::mosaic_t::dump_line(ostream& os) const {
	os << id << ' ';
	os << amount << ' ';
}

void account_t::account_data_t::dump_fields(ostream& os) {
	os << "version address addressHeight publicKey publicKeyHeight accountType supplementalPublicKeys activityBuckets ";
	os << "[ ";
	mosaic_t::dump_fields(os);
	os << "] ";
	os << "importance importanceHeight id ";
}

void account_t::account_data_t::dump_line(ostream& os) const {
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
	for (auto&i:mosaics) i.dump_line(os);
	os << importance << ' ';
	os << importanceHeight << ' ';
	os << id << ' ';
}

bool account_t::account_data_t::from_json(account_t::account_data_t& o, const string& json) {
	return SerializeFromJson(o, json);
}

void account_t::account_data_t::to_json(ostream& os) const {
	SerializeToJson(*this, os);
	os << '\n';
}

void account_t::dump_fields(ostream& os) {
	account_t::dump_fields(os);
	os << '\n';
}

void account_t::dump(ostream& os) const {
	account.dump_line(os);
	os << '\n';
}

bool account_t::from_json(const string& json) {
	return SerializeFromJson(*this, json);
}

void account_t::to_json(ostream& os) const {
	SerializeToJson(*this, os);
	os << '\n';
}

bool account_t::fetch(const string& url, const string& account) {
//cout << account << endl;
	return ufetch(*this, url+uri+'/'+account, [](const string& i) -> string { return i; });
}


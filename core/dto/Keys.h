#pragma once

#include "../base.h"
#include "json.h"

namespace symbol { namespace core {namespace dto {

	struct Keys {
		static constexpr auto Json_Element="keys";
		uint8_t networkId;
		string network;
		string privateKey;
		string publicKey;
		string address;
		string account;

		static void dumpFields(ostream&);
		void dumpLine(ostream&) const;

		void toText(const string& name, bool compact, const string& indent, ostream&) const;

		ko fromJson(const rapidjson::Value&);
		ko fromJson(const string&);
		void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType&) const;
		void toJson(bool compact, ostream&) const;
		string toJson(bool compact) const;
		vector<uint8_t> toBin() const;
	};

}}}


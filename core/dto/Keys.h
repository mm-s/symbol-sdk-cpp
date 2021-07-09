#pragma once

#include "../base.h"
#include "json.h"

namespace symbol { namespace core {namespace dto {

	struct Keys {
		static constexpr auto Json_Element="keys";
		string networkId;
		string network;
		string privateKey;
		string publicKey;
		string address;
		string account;

		static void dumpFields(ostream&);
		void dumpLine(ostream&) const;

		void toText(bool compact, ostream&) const;

		ko fromJson(const rapidjson::Value&);
		ko fromJson(const string&);
		void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType&) const;
		void toJson(bool compact, ostream&) const;
		string toJson(bool compact) const;
	};

}}}

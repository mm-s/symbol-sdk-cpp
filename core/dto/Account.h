#pragma once

#include "../base.h"
#include "json.h"

namespace symbol { namespace core {namespace dto {

	struct Account {
		static constexpr auto Json_Element="account";
		struct Mosaic {
			string id;
			string amount;
			void dumpLine(ostream&) const;
			static void dumpFields(ostream&);
			void toText(bool compact, ostream&) const;
			ko fromJson(const rapidjson::Value&);
			void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType&) const;
		};
		int version;
		string address;
		string addressHeight;
		string publicKey;
		string publicKeyHeight;
		int accountType;
		vector<string> supplementalPublicKeys;
		vector<string> activityBuckets;
		vector<Mosaic> mosaics;
		string importance;
		string importanceHeight;
		string id;

		static void dumpFields(ostream&);
		void dumpLine(ostream&) const;

		void toText(bool compact, ostream&) const;

		ko fromJson(const rapidjson::Value&);
		ko fromJson(const string&);
		void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType&) const;
		void toJson(bool compact, ostream&) const;
		string toJson(bool compact) const;
	};

}}} // namespaces


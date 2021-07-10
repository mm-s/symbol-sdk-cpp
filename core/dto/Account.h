#pragma once

#include "../base.h"
#include "json.h"

namespace symbol { namespace core {namespace dto {

	struct Account {
		struct AccountData {
			static constexpr auto Json_Element="account";
			struct Mosaic {
				string id;
				string amount;
				//void dumpLine(ostream&) const;
				static void dumpFields(ostream&);
				void toText(bool compact, ostream&) const;
				ko fromJson(const rapidjson::Value&);
				void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType&) const;
			};
			struct SupplementalPublicKeys: vector<string> {
				using b = vector<string>;
				//void dumpLine(ostream&) const;
				static void dumpFields(ostream&);
				void toText(bool compact, ostream&) const;
				ko fromJson(const rapidjson::Value&);
				void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType&) const;
			};
			uint16_t version;
			string address;
			string addressHeight;
			string publicKey;
			string publicKeyHeight;
			uint8_t accountType;
			SupplementalPublicKeys supplementalPublicKeys;
			vector<string> activityBuckets;
			vector<Mosaic> mosaics;
			string importance;
			string importanceHeight;

			static void dumpFields(ostream&);
			void toText(bool compact, ostream&) const;
			ko fromJson(const rapidjson::Value&);
			void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType&) const;
		};
		
		AccountData account;
		string id;

		static void dumpFields(ostream&);
		void toText(bool compact, ostream&) const;
		ko fromJson(const rapidjson::Value&);
		void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType&) const;
		void toJson(bool compact, ostream&) const;
		string toJson(bool compact) const;
		vector<uint8_t> toBin() const;
		size_t size() const;
		void toBin(vector<uint8_t>&, size_t pos) const;

		ko fromJson(const string&);
	};

}}} // namespaces


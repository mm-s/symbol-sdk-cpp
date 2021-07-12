#pragma once

#include "../base.h"
#include "json.h"

namespace symbol { namespace core { namespace dto {

#pragma pack(push, 1)

	struct NetNode {
		struct PeerStatus {
			static constexpr auto Json_Element="peerStatus";
			bool isAvailable;
			uint64_t lastStatusCheck;
			
			void dumpLine(ostream&) const;
			static void dumpFields(ostream&);
			void toText(const string& name, bool compact, const string& indent, ostream&) const;
			ko fromJson(const rapidjson::Value&);
			void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) const;
		};
		
		struct ApiStatus {
			static constexpr auto Json_Element="apiStatus";
			bool isAvailable;
			uint64_t chainHeight;
			uint64_t finalizationHeight;
			string nodePublicKey;
			uint64_t lastStatusCheck;
			
			void dumpLine(ostream&) const;
			static void dumpFields(ostream&);
			void toText(const string& name, bool compact, const string& indent, ostream&) const;
			ko fromJson(const rapidjson::Value&);
			void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) const;
		};
		
		struct HostDetail {
			static constexpr auto Json_Element="hostDetail";
			struct Coordinates {
				static constexpr auto Json_Element="coordinates";
				double latitude;
				double longitude;

				void toText(const string& name, bool compact, const string& indent, ostream&) const;
				ko fromJson(const rapidjson::Value&);
				void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) const;
			};
			string host;
			Coordinates coordinates;
			string location;
			string ip;
			string organization;
			string as;
			string continent;
			string country;
			string region;
			string city;
			string district;
			string zip;

			void dumpLine(ostream&) const;
			static void dumpFields(ostream&);
			void toText(const string& name, bool compact, const string& indent, ostream&) const;
			ko fromJson(const rapidjson::Value&);
			void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) const;
		};
		
		struct RewardProgram {
			string _id;
			string name;
			bool passed;

			void toText(const string& name, bool compact, const string& indent, ostream&) const;
			ko fromJson(const rapidjson::Value&);
			void dumpLine(ostream&) const;
			static void dumpFields(ostream&);
			void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) const;
		};
		
		PeerStatus peerStatus;
		ApiStatus apiStatus;
		string _id;
		int version;
		string publicKey;
		string networkGenerationHashSeed;
		int roles;
		int port;
		int networkIdentifier;
		string host;
		string friendlyName;
		vector<RewardProgram> rewardPrograms;
		HostDetail hostDetail;
		int __v;

		static void dumpFields(ostream&);
//		void dumpLine(ostream&) const;
		void toText(const string& name, bool compact, const string& indent, ostream&) const;

		ko fromJson(const rapidjson::Value&);
		void toJson(ostream&) const;
		void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType&) const;
	};

	struct NetNodes: vector<NetNode> {
		using b=vector<NetNode>;
		static constexpr auto Json_Element="nodes";

		static void dumpFields(ostream&);
		void dump(ostream&) const;

//		static void dumpFields(ostream& os);

		void toText(const string& name, bool compact, const string& indent, ostream&) const;
		ko fromJson(const string&);
		ko fromJson(const rapidjson::Value&);
		void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType&) const;
		void toJson(bool compact, ostream&) const;
		string toJson(bool compact) const;
		vector<uint8_t> toBin() const;

	};

#pragma pack(pop)

}}} // namespaces


#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <rapidjson/document.h>

namespace symbol { namespace dto {
	using std::ostream;
	using std::string;
	using std::vector;

	struct net_node {
		struct peerStatus_t {
			static constexpr auto Json_Element="peerStatus";
			bool isAvailable;
			uint64_t lastStatusCheck;
			
			void dump_line(ostream&) const;
			static void dump_fields(ostream&);
			bool from_json(const rapidjson::Value&);
		};
		struct apiStatus_t {
			static constexpr auto Json_Element="apiStatus";
			bool isAvailable;
			uint64_t chainHeight;
			uint64_t finalizationHeight;
			string nodePublicKey;
			uint64_t lastStatusCheck;
			
			void dump_line(ostream&) const;
			static void dump_fields(ostream&);
			bool from_json(const rapidjson::Value&);

		};
		struct hostDetail_t {
			static constexpr auto Json_Element="hostDetail";
			struct coordinates_t {
				static constexpr auto Json_Element="coordinates";
				double latitude;
				double longitude;

				bool from_json(const rapidjson::Value&);
			};
			string host;
			coordinates_t coordinates;
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

			void dump_line(ostream&) const;
			static void dump_fields(ostream&);
			bool from_json(const rapidjson::Value&);
		};
		
		struct RewardProgram {
			string _id;
			string name;
			bool passed;

			bool from_json(const rapidjson::Value&);
			void dump_line(ostream&) const;
			static void dump_fields(ostream&);
		};
		
		peerStatus_t peerStatus;
		apiStatus_t apiStatus;
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
		hostDetail_t hostDetail;
		int __v;

		static void dump_fields(ostream&);
		void dump_line(ostream&) const;

		bool from_json(const rapidjson::Value&);
		bool from_json(const string&);
		void to_json(ostream&) const;
	};

	struct nodes: vector<net_node> {
		static constexpr auto Json_Element="nodes";

		static void dump_fields(ostream&);
		void dump(ostream&) const;

		bool from_json(const string&);
		bool from_json(const rapidjson::Value&);
		void to_json(ostream&) const;
		bool fetch(const string& url);
		
	};

}} // namespaces


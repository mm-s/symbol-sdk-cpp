#pragma once

#include "../base.h"
#include "json.h"

namespace symbol { namespace core { namespace dto {

	struct ApiNode {
		struct Health {
			struct Status {
				string apiNode;
				string db;
				void dump(ostream&) const;
				ko fromJson(const rapidjson::Value&);
				void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType&) const;
			};
			Status status;

			void dump(ostream&) const;
			ko fromJson(const string&);
			ko fromJson(const rapidjson::Value&);
			void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType&) const;
			void toJson(bool pretty, ostream&) const;
		};

		struct Peer {
			int version;
			string publicKey;
			string networkGenerationHashSeed;
			int roles;
			int port;
			int networkIdentifier;
			string host;
			string friendlyName;

			static void dumpFields(ostream&);
			virtual void dumpTable(ostream&) const;
			virtual void dumpLine(ostream&) const;

			ko fromJson(const rapidjson::Value&);
			void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType&) const;
		};

		struct Peers: vector<Peer> {
			using b=vector<Peer>;
			static constexpr auto Json_Element="peers";
			static void dumpFields(ostream&);
			void dump(ostream&) const;


			ko fromJson(const rapidjson::Value&);
			ko fromJson(const string&);
			void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType&) const;
			void toJson(bool pretty, ostream&) const;
		};

		struct Info: Peer {
			using b = Peer;
			string nodePublicKey;
			//static constexpr auto Json_Element="nodes";

			void dumpTable(ostream&) const override;
			void dumpLine(ostream&) const override;

			ko fromJson(const rapidjson::Value&);
			ko fromJson(const string&);
			void toJson(rapidjson::Value& parent, rapidjson::Document::AllocatorType&) const;
			void toJson(bool pretty, ostream&) const;
			string toJson(bool pretty) const;
		};
	};

}}} // namespaces


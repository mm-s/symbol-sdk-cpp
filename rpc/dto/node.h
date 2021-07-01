#pragma once

#include <string>
#include <iostream>
#include <vector>

namespace symbol { namespace dto {
	using std::ostream;
	using std::string;
	using std::vector;

	struct node_health {
		struct status_t {   
			string apiNode;
			string db;
			void dump(ostream&) const;
			static bool from_json(node_health::status_t&, const string&);
			void to_json(ostream&) const;
		};
		status_t status;

		void dump(ostream&) const;
		static bool from_json(node_health&, const string&);
		void to_json(ostream&) const;
		bool fetch(const string& url);
		static constexpr const char* uri{"/node/health"};
	};

	struct node_peer {
		int version;
		string publicKey;
		string networkGenerationHashSeed;
		int roles;
		int port;
		int networkIdentifier;
		string host;
		string friendlyName;

		static void dump_fields(ostream&);
		virtual void dump_table(ostream&) const;
		virtual void dump_line(ostream&) const;

		static bool from_json(node_peer&, const string&);
		void to_json(ostream&) const;
	};

	struct node_peers {
		vector<node_peer> peers;
		static void dump_fields(ostream&);
		void dump(ostream&) const;


		static bool from_json(node_peers&, const string&);
		void to_json(ostream&) const;
		bool fetch(const string& url);
		static constexpr const char* uri{"/node/peers"};
	};

	struct node_info: node_peer {
		using b = node_peer;
		string nodePublicKey;

		void dump_table(ostream&) const override;
		void dump_line(ostream&) const override;

		static bool from_json(node_info&, const string&);
		void to_json(ostream&) const;
		bool fetch(const string& url);
		static constexpr const char* uri{"/node/info"};

	};

}} // namespaces


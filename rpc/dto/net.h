#pragma once

#include <string>
#include <iostream>
#include <vector>

namespace symbol { namespace dto {
	using std::ostream;
	using std::string;
	using std::vector;

    struct net_node {
        struct peerStatus_t {
            bool isAvailable;
            uint64_t lastStatusCheck;
            
            void dump_line(ostream&) const;
            static void dump_fields(ostream&);
        };
        struct apiStatus_t {
            bool isAvailable;
            uint64_t chainHeight;
            uint64_t finalizationHeight;
            string nodePublicKey;
            uint64_t lastStatusCheck;
            
            void dump_line(ostream&) const;
            static void dump_fields(ostream&);

        };
        struct hostDetail_t {
            struct coordinates_t {
                double latitude;
                double longitude;
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
        vector<string> rewardPrograms;
        hostDetail_t hostDetail;
        int __v;
        
        static void dump_fields(ostream&);
        void dump_line(ostream&) const;

        static bool from_json(net_node&, const string&);
        void to_json(ostream&) const;
    };

    struct net_list {
        vector<net_node> nodes;
        static void dump_fields(ostream&);
        void dump(ostream&) const;


        static bool from_json(net_list&, const string&);
        void to_json(ostream&) const;
        bool fetch(const string& url);
        
    };

}} // namespaces


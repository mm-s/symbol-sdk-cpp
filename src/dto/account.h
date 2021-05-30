#pragma once

#include <string>
#include <iostream>
#include <vector>

namespace symbol {
namespace dto {
    using namespace std;

    struct account_t {
        struct account_data_t {
            struct mosaic_t {
                string id;
                string amount;

                void dump_line(ostream&) const;
                static void dump_fields(ostream&);
            };
            int version;
            string address;
            string addressHeight;
            string publicKey;
            string publicKeyHeight;
            int accountType;
            vector<string> supplementalPublicKeys;
            vector<string> activityBuckets;
            vector<mosaic_t> mosaics;
            string importance;
            string importanceHeight;
            string id;

            static void dump_fields(ostream&);
            void dump_line(ostream&) const;
            static bool from_json(account_data_t&, const string&);
            void to_json(ostream&) const;
        };
        account_data_t account;
        
        static void dump_fields(ostream&);
        void dump(ostream&) const;
        static bool from_json(account_t&, const string&);
        void to_json(ostream&) const;

        bool fetch(const string& url, const string& account);
        static constexpr const char* uri{"/accounts"};
        
    };

}
}

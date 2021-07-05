#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <restc-cpp/SerializeJson.h>
#include <functional>

namespace symbol { namespace dto {
	using std::ostream;
	using std::string;
	using std::vector;
	using std::function;
	using std::exception;
	using std::move;
	using std::istringstream;
	using Context=restc_cpp::Context;

	template<typename t>
	bool ufetch(t&o, const string& url, function<string(const string&)> json_transform=[](const string& i) -> string { return i; }) {
		auto cli = restc_cpp::RestClient::Create();
		auto f=cli->ProcessWithPromiseT<t>([url, json_transform](Context& ctx) {
			t o;
			o.from_json(json_transform(ctx.Get(url)->GetBodyAsString()));
			return move(o);
		});
		bool r{true};
		try {
			o = move(f.get());
		}
		catch(const exception& ex) {
			r = false;
		}
		cli->CloseWhenReady(true);
		return r;
	}

	using restc_cpp::SerializeFromJson;

	template<typename t>
	bool SerializeFromJson(t& o, const string& json) {
		istringstream is(json);
		SerializeFromJson(o, is);
		return true;
	}

}} // namespace


#include "json.h"
#include <rapidjson/reader.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

namespace symbol { namespace core { namespace dto {

	template<>
	void toText(bool compact, const vector<string>& v, ostream& os) {
		if (compact) {
			os << v.size() << ' ';
			for (auto&i:v) os << i << ' ';
		}
		else {
			int n=0;
			for (auto&i:v) {
				os << "  #" << n++ << ": " << i << '\n';
			}
		}
	}

	string toString(const rapidjson::Value& v, bool pretty) {
		using namespace rapidjson;
		string r;
		if (pretty) {
			StringBuffer buffer;
			PrettyWriter<StringBuffer> w(buffer);
			v.Accept(w);
			r=buffer.GetString();
		}
		else {
			StringBuffer buffer;
			Writer<StringBuffer> w(buffer);
			v.Accept(w);
			r=buffer.GetString();
		}
		return move(r);
	}

	template<>
	bool readField(string& dest, const rapidjson::Value& v, const char* jsonElement) {
		rapidjson::Value::ConstMemberIterator itr = v.FindMember(jsonElement);
		if (itr == v.MemberEnd()) {
			dest="";
			return false;
		}
		dest = itr->value.GetString();
		return true;
	}

	template<>
	bool readField(bool& dest, const rapidjson::Value& v, const char* jsonElement) {
		rapidjson::Value::ConstMemberIterator itr = v.FindMember(jsonElement);
		if (itr == v.MemberEnd()) {
			dest=false;
			return false;
		}
		dest = itr->value.GetBool();
		return true;
	}

	template<>
	bool readField(int& dest, const rapidjson::Value& v, const char* jsonElement) {
		rapidjson::Value::ConstMemberIterator itr = v.FindMember(jsonElement);
		if (itr == v.MemberEnd()) {
			dest=0;
			return false;
		}
		dest = itr->value.GetInt();
		return true;
	}

	template<>
	bool readField(double& dest, const rapidjson::Value& v, const char* jsonElement) {
		rapidjson::Value::ConstMemberIterator itr = v.FindMember(jsonElement);
		if (itr == v.MemberEnd()) {
			dest=0;
			return false;
		}
		dest = itr->value.GetDouble();
		return true;
	}

	template<>
	bool readField(uint64_t& dest, const rapidjson::Value& v, const char* jsonElement) {
		rapidjson::Value::ConstMemberIterator itr = v.FindMember(jsonElement);
		if (itr == v.MemberEnd()) {
			dest=0;
			return false;
		}
		dest = itr->value.GetUint64();
		return true;
	}

	template<>
	bool readField(vector<string>&o, const rapidjson::Value& v, const char* jsonElement) {
		o.clear();
		rapidjson::Value::ConstMemberIterator itr = v.FindMember(jsonElement);
		if (itr == v.MemberEnd()) return false;
		auto& V=itr->value;
		if (!V.IsArray()) return false;
		o.reserve(V.Size());
		for (rapidjson::Value::ConstValueIterator itr2 = V.Begin(); itr2 != V.End(); ++itr2) {
			if (!itr2->IsString()) return false;
			o.push_back(itr2->GetString());
		}
		return true;
	}


	template<>
	void writeField(const char* jsonElement, const string& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) {
		rapidjson::Value v;
		v.SetString(rapidjson::StringRef(o.c_str()));
		parent.AddMember(rapidjson::StringRef(jsonElement), v, ator);
	}

	template<>
	void writeField(const char* jsonElement, const bool& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) {
		rapidjson::Value v;
		v.SetBool(o);
		parent.AddMember(rapidjson::StringRef(jsonElement), v, ator);
	}

	template<>
	void writeField(const char* jsonElement, const int& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) {
		rapidjson::Value v;
		v.SetInt(o);
		parent.AddMember(rapidjson::StringRef(jsonElement), v, ator);
	}

	template<>
	void writeField(const char* jsonElement, const uint64_t& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) {
		rapidjson::Value v;
		v.SetUint64(o);
		parent.AddMember(rapidjson::StringRef(jsonElement), v, ator);
	}

	template<>
	void writeField(const char* jsonElement, const double& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) {
		rapidjson::Value v;
		v.SetDouble(o);
		parent.AddMember(rapidjson::StringRef(jsonElement), v, ator);
	}

	template<>
	void writeField(const char* jsonElement, const vector<string>& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) {
		rapidjson::Value array;
		array.SetArray();
		for (auto&i:o) {
			rapidjson::Value v;
			v.SetString(rapidjson::StringRef(i.c_str()));
			parent.AddMember(rapidjson::StringRef(jsonElement), v, ator);
			array.PushBack(v, ator);
		}
		parent.AddMember(rapidjson::StringRef(jsonElement), array, ator);
	}

}}} // namespaces

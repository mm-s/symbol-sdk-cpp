#pragma once

#include "../base.h"
#include <string>
#include <iostream>
#include <vector>
#include <rapidjson/document.h>

namespace symbol { namespace core { namespace dto {

	template<typename T>
	void toText(bool compact, const vector<T>& v, ostream& os) {
		if (compact) {
			os << v.size() << ' ';
			for (auto&i:v) i.toText(compact, os);
		}
		else {
			int n=0;
			for (auto&i:v) {
				os << "  #" << n++ << ": \n";
				i.toText(compact, os);
			}
		}
	}

	template<>
	void toText(bool compact, const vector<string>& v, ostream&);

	string toString(const rapidjson::Value& v, bool pretty);

	template<typename T>
	bool readField(T&o, const rapidjson::Value& v, const char* jsonElement) {
		rapidjson::Value::ConstMemberIterator itr = v.FindMember(jsonElement);
		if (itr == v.MemberEnd()) return false;
		o.fromJson(itr->value);
		return true;
	}

	template<typename T>
	bool readField(vector<T>&o, const rapidjson::Value& v, const char* jsonElement) {
		o.clear();
		rapidjson::Value::ConstMemberIterator itr = v.FindMember(jsonElement);
		if (itr == v.MemberEnd()) return false;
		auto& V=itr->value;
		if (!V.IsArray()) return false;
		o.reserve(V.Size());
		for (rapidjson::Value::ConstValueIterator itr2 = V.Begin(); itr2 != V.End(); ++itr2) {
			T i;
			i.fromJson(*itr2);
			o.push_back(i);
		}
		return true;
	}

	template<>
	bool readField(vector<string>&o, const rapidjson::Value& v, const char* jsonElement);

	template<> bool readField(string& dest, const rapidjson::Value&, const char* jsonElement);
	template<> bool readField(bool& dest, const rapidjson::Value&, const char* jsonElement);
	template<> bool readField(int& dest, const rapidjson::Value&, const char* jsonElement);
	template<> bool readField(double& dest, const rapidjson::Value&, const char* jsonElement);
	template<> bool readField(uint8_t& dest, const rapidjson::Value&, const char* jsonElement);
	template<> bool readField(uint16_t& dest, const rapidjson::Value&, const char* jsonElement);
	template<> bool readField(uint32_t& dest, const rapidjson::Value&, const char* jsonElement);
	template<> bool readField(uint64_t& dest, const rapidjson::Value&, const char* jsonElement);

	template<typename T>
	void writeField(const char* jsonElement, const T& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) {
		rapidjson::Value v(rapidjson::kObjectType);
		o.toJson(v, ator);
		parent.AddMember(rapidjson::StringRef(jsonElement), v, ator);
	}

	template<typename T>
	void writeField(const char* jsonElement, const vector<T>& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator) {
		rapidjson::Value array;
		array.SetArray();
		for (auto&i:o) {
			rapidjson::Value v;
			v.SetObject();
			i.toJson(v, ator);
			array.PushBack(v, ator);
		}
		parent.AddMember(rapidjson::StringRef(jsonElement), array, ator);
	}

	template<> void writeField(const char* jsonElement, const vector<string>& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator);

	template<> void writeField(const char* jsonElement, const string& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator);
	template<> void writeField(const char* jsonElement, const bool& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator);
	template<> void writeField(const char* jsonElement, const int& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator);
	template<> void writeField(const char* jsonElement, const double& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator);
	template<> void writeField(const char* jsonElement, const uint8_t& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator);
	template<> void writeField(const char* jsonElement, const uint16_t& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator);
	template<> void writeField(const char* jsonElement, const uint32_t& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator);
	template<> void writeField(const char* jsonElement, const uint64_t& o, rapidjson::Value& parent, rapidjson::Document::AllocatorType& ator);


}}} // namespaces


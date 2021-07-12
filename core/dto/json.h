#pragma once

#include "../base.h"
#include <string>
#include <iostream>
#include <vector>
#include <rapidjson/document.h>

namespace symbol { namespace core { namespace dto {

	// ----------- write text

	template<typename T>
	void writeField(const string& name, const T& v, bool compact, const string& indent, ostream& os) {
		if (compact) {
			v.toText(name, compact, indent, os);
//			os << v << ' ';
		}
		else {
			string ind=indent+"  ";
			os << indent << name << '\n';
			v.toText("", compact, ind, os);
//			os << indent << name << ' ' << v << '\n';
		}
	}

	template<> void writeField(const string& name, const string& v, bool compact, const string& indent, ostream&);
	template<> void writeField(const string& name, const bool&, bool compact, const string& indent, ostream&);
	template<> void writeField(const string& name, const int&, bool compact, const string& indent, ostream&);
	template<> void writeField(const string& name, const double&, bool compact, const string& indent, ostream&);
	template<> void writeField(const string& name, const uint8_t&, bool compact, const string& indent, ostream&);
	template<> void writeField(const string& name, const uint16_t&, bool compact, const string& indent, ostream&);
	template<> void writeField(const string& name, const uint32_t&, bool compact, const string& indent, ostream&);
	template<> void writeField(const string& name, const uint64_t&, bool compact, const string& indent, ostream&);

	template<typename T>
	void writeField(const string& name, const vector<T>& v, bool compact, const string& indent, ostream& os) {
		if (compact) {
			os << v.size() << ' ';
			for (auto&i:v) i.toText("", compact, indent, os);
		}
		else {
			int n=0;
			os << indent << name << '\n';
			string ind=indent+"  ";
			string ind2=ind+"  ";
			for (auto&i:v) {
				os << ind << "#" << n++ << ": \n";
				i.toText("", compact, ind2, os);
			}
		}
	}

	template<> void writeField(const string& name, const vector<string>& v, bool compact, const string& indent, ostream&);

	string toString(const rapidjson::Value& v, bool pretty);

	// ----------- read json

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

	// ----------- write json

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


	// ----------- write bin

	template<typename T>
	size_t binSize(const T& o) {
		return 0;
	}

	template<> size_t binSize(const string& o);

	template<typename T>
	void writeBin(const T& o) {
	}


}}} // namespaces


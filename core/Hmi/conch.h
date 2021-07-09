/**
*** Copyright (c) 2016-2019, Jaguar0625, gimre, BloodyRookie, Tech Bureau, Corp.
*** Copyright (c) 2020-present, Jaguar0625, gimre, BloodyRookie.
*** All rights reserved.
***
*** This file is part of Catapult.
***
*** Catapult is free software: you can redistribute it and/or modify
*** it under the terms of the GNU Lesser General Public License as published by
*** the Free Software Foundation, either version 3 of the License, or
*** (at your option) any later version.
***
*** Catapult is distributed in the hope that it will be useful,
*** but WITHOUT ANY WARRANTY; without even the implied warranty of
*** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*** GNU Lesser General Public License for more details.
***
*** You should have received a copy of the GNU Lesser General Public License
*** along with Catapult. If not, see <http://www.gnu.org/licenses/>.
**/
#pragma once
#include <vector>
#include <string>
#include <functional>
#include <set>
#include <iostream>

/// Shell program-options.
/// This class encapsulates the console UI behaviour.
namespace conch {
	using namespace std;

	/// Flag spec
	struct flagdef {
		char short_name;
		string name;
		bool optional;
		bool req_input;
		string value;
		string desc;
		bool overriden{false};

		bool operator == (const string&) const; ///string includes - or -- prefixes
		bool operator == (const flagdef&) const;
		void dump(const string& pfx, ostream&) const;
		void dump_valuex(const string& pfx, ostream&) const;
	};

	/// Command (a.k.a. menuitem) spec
	struct cmddef {
		string name;
		string desc;
		bool operator == (const string&) const;
		bool operator == (const cmddef&) const;
		void dump(const string& pfx, ostream&) const;
	};

	class section;
	/// Specification of the command params (a.k.a. flags)
	struct params: vector<flagdef> {
		using b = vector<flagdef>;
		using b::vector;
		params(b&&);
		params(const params&, istream&);
		flagdef* lookup_flag_from_input(const string& ui_input);
		flagdef* lookup(char);
		const string& get(const string&) const;
		const string& get(char) const;
		bool is_set(char flag) const;
		bool is_set(const string& flag) const;
		bool any_set() const;
		bool any_set(const section& ignoresrc) const;
		bool has(char flag) const;
		void set_optional(char flag);
		void set_mandatory(char flag);
		void dump(const string& prefix, ostream&) const;
		void dump_values(const string& prefix, ostream&) const;
		void dump_set(const string& prefix, ostream&) const;
		void dump_set(const string& prefix, const section& ignoresrc, ostream&) const;
		bool check_req(ostream&) const;
		bool check_unique() const;
		const flagdef* flag_req_help() const;
		string ko;
	};

	/// Section of an UI represents a menu item with its exec logic and children subtree of menuitems.
	/// Not thread-safe. Uses static data. 
	class section: private vector<pair<cmddef, section*>> {
		using b = vector<pair<cmddef, section*>>;

	public:
		using handler_t = function<bool(params&, bool last, ostream&)>;

		/// Linear path over the tree. Sequence of choices.
		struct param_path: vector<pair<section*, params*>> {
			params* lookup(const vector<string>& cmdpath);
		};

	public:
		/// Static initialization
		static void init(ostream&, ostream&, istream&, const string& version);

		/// Construction, Initialization, Destruction
		section();
		section(params&&);
		section(handler_t);
		void constructor();
		virtual ~section();

		virtual void init(const string& name, const string& description);

	public:
		///Composing the choices tree
		section& add(cmddef&&, section*);
		bool remove(const string& cmd);
		bool disable(const string& cmd);
		void add(flagdef&&);
		void add(params&&);
		void set_handler(function<bool(params&, bool, ostream&)>);

		/// Rewrite params and flags spec before excecuting command. Called before executing the first command. Writable ParamPath can be used to tweak the spec based on user choices. e.g. A flag A is optional only if flag B is set otherwise is mandatory.
		virtual bool pass1(param_path&, ostream&);

		///Excecuting command
		bool exec(istream&);
		bool exec(const string&);

		virtual void help_flag(const flagdef& f, ostream& os) const;

	public:
		const section* root() const;
		section* root();

	private:
		bool exec(const param_path&, param_path::const_iterator);
		void help(const param_path&, ostream&) const;
		void help(const param_path&, param_path::const_iterator, ostream&) const;
		string scope() const;
		bool fillv(param_path&, istream&);
		section* lookup(const string&) const;
		const section* lookup(const param_path&, param_path::const_iterator) const;
		section* lookup(const param_path&, param_path::const_iterator);
//		param_path::const_iterator check_req(const param_path&, ostream&) const;
		inline bool isRoot() const { return parent == nullptr; }
		static void print_error(const string& msg);
		bool pass2(param_path&, ostream&);

	public:
		inline const string& name() const { return m_name; }
		inline const string& desc() const { return m_desc; }
		
	public:
		bool ignore(char flag) const;
		set<char> ignoreFlags; /// Used to hide unused contextual flags (coming from inherited sections) in the help screen.
		
		static istream* pis;
	private:
		static ostream* pos;
		static ostream* peos;
		static string version;
		string m_name;
		string m_desc;
		params m_pdef;
		bool enabled{true};
		handler_t handler;
		bool skip_handler{true};
		section* parent{nullptr};
	};
}

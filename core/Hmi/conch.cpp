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
#include "conch.h"
#include <iostream>
#include <sstream>
#include <cassert>

using c=conch::section;
using namespace conch;
using namespace std;

ostream* c::pos{nullptr};
ostream* c::peos{nullptr};
string c::version;

namespace {
	string fmt_field(const string& x, int w) {
		ostringstream os;
		if (x.size() >= w) {
			os << x << ' ';
			return os.str();
		}
		os << x << string(w, ' ');
		return os.str().substr(0, w);
	}
}

void c::init(ostream& out, ostream& err, const string& v) {
	pos = &cout;
	peos = &cerr;
	version = v;
}

namespace {
	constexpr int tabpos=35;
}

c::section(params&& p): m_pdef(p) {
	constructor();
}

c::section(): m_pdef({}) {
	constructor();
}

c::section(function<bool(params&, ostream&)>f): m_pdef({}), handler(f) {
	assert(m_pdef.check_unique());
}

void c::constructor() {
	auto& def = const_cast<params&>(m_pdef);
	def.emplace_back(flagdef{'h', "help", true, false, "", "Prints this help."});
	handler = [](params& p, ostream& os) -> bool {
		os << "Not implemented.\n";
		return false;
	};
}

void c::init(const string& nm, const string& dc) {
	m_name = nm;
	m_desc = dc;
}

void c::add(flagdef&& fd) {
	assert(!m_pdef.empty());
	auto& def = const_cast<params&>(m_pdef);
	auto it = --(def.rbegin().base());
	def.emplace(it, move(fd));
	assert(m_pdef.check_unique());
}

c::~section() {
	for (auto& i:*this) delete i.second;
}

c& c::add(cmddef&& a, section* s) {
	assert( s != nullptr );
	assert( s != this );
	assert( s->parent == nullptr );
	assert(s->m_pdef.check_unique());
	s->parent = this;
	s->init(a.name, a.desc);
	emplace_back(make_pair(a, s));
	assert(m_pdef.check_unique());
	assert(s->m_pdef.check_unique());
	return *s;
}

string c::scope() const {
	if (parent == nullptr) return m_name;
	return parent->scope() + " " + m_name;
}

void c::help(const param_path& v) const {
	assert(!v.empty());
	help(v, --v.end());
}

void c::help(const param_path& v, param_path::const_iterator i) const {
	assert(!v.empty());
	auto& os = *pos;
	params* pp = nullptr;
	for (auto j = v.begin(); j != v.end(); ++j) { //override ancestor page if forced -h
		if (j->second->is_set('h')) {
			i = j;
			break;
		}
	}
	pp = i->second;
	auto& p = *pp;
	auto* s = lookup(v, i);
	assert(s != nullptr);
	string sc = s->scope();
	os << sc << "  r" << version << '\n';
	os << '\n';
	os << m_desc << '\n';
	os << '\n';
	{
		ostringstream buf;
		const section& secignore = *v.rbegin()->first;
		for (auto j = v.begin(); j != i; ++j) {
			string ind; /// Print section name.
			ostringstream bufT;
			if (!(*j).first->isRoot()) {
				ind = "  ";
				bufT << ind << (*j).first->name() << ":\n";
			}
			ostringstream bufB;
			(*j).second->dump_set(ind + "  ", secignore, bufB); /// Print flag
			auto bb = bufB.str();
			if (!bb.empty()) {
				buf << bufT.str() << bb;
			}
		}
		auto bufstr = buf.str();
		if (!bufstr.empty()) {
			os << "context:\n";
			os << bufstr << '\n';
		}
	}
	os << "usage:\n";
	os << "  " << sc << ' ';
	if (!p.empty()) {
		os << "[flags] ";
	}
	if (!s->empty()) {
		os << "<command> [-h]";
	}
	os << '\n';
	p.dump("", os);
	if (!s->empty()) {
		os << '\n';
		os << "commands:\n";
		for (auto& i: *s) {
			os << "  " << fmt_field(i.first.name, tabpos) << i.first.desc << '\n';
		}
	}
}

void c::set_handler(function<bool(params&, ostream&)> f) {
	handler=f;
	skip_handler=false;
}

bool c::fillv(param_path& v, istream& is) {
	assert(m_pdef.check_unique());
	params* p = new params(m_pdef, is);
	v.emplace_back(make_pair(this, p));
	if (!p->ko.empty()) {
		print_error(p->ko);
		help(v);
		return false;
	}
	string cmd;
	is >> cmd;
	if (cmd.empty() && empty()) return true;
	auto s = lookup(cmd);
	if (s == nullptr) {
		if (!cmd.empty()) {
			print_error(string("Unexpected command. '") + cmd + "'.");
			help(v);
			return false;
		}
		return true; //ok
/*
		if (empty()) { /// Section hasn't commands
			if (cmd.empty()) {
				return true; //ok
			}
		}
		if (!cmd.empty()) {
			print_error(string("Unexpected command. '") + cmd + "'.");
			help(v);
			return false;
		}
*/
	}
	//if (cmd.empty()) {
	//	return true;
	//}
	return s->fillv(v, is);
}

bool c::exec(const param_path& v, param_path::const_iterator i) {
	params* p = i->second;
	if (p->is_set('h')) {
		auto n = i+1;
		if (n != v.end()) {
			print_error(string("Invoking help caused command '") + n->first->name() + "' to be ignored.");
		}
		help(v, i);
		return true;
	}
	{
		ostringstream os;
		if (!i->second->check_req(os)) {
//		auto it = check_req(v, os);
//		if (it != v.end() && it <= i) {
			print_error(os.str());
			help(v, i);
			return false;
		}
	}
	if (!skip_handler) {
		ostringstream os;
		auto r = handler(*p, os);
		if (r) {
			*pos << os.str();
		}
		else {
			*peos << "ERR " << os.str() << '\n';
		}
		if (!r) return false;
	}
	++i;
	if (i == v.end()) {
		if (!empty()) {
			print_error("Missing command.");
			help(v);
			return false;
		}
		return true;
	}
	/*
	auto s = lookup(i->first->name);
	if (s == nullptr) {
		print_error(i->first.empty() ? "Missing command." : string("Invalid command '") + i->first + "'.");
		help(v);
		return false;
	}
	*/
	assert(i->first != nullptr);
	return i->first->exec(v, i);
	//return s->exec(v, i);
}

bool c::exec(const string& cmdline) { 
	istringstream is(cmdline);
	return exec(is);
}

/*
c::param_path::const_iterator c::check_req(const param_path& v, ostream&os) const {
	auto it = v.end();
	for (auto i = v.begin(); i!=v.end(); ++i) {
		if (!i->second->check_req(os)) {
			if (it == v.end()) it = i;
		}
	}
	return it;
}
*/

params* c::param_path::lookup(const vector<string>& cmdpath) {
	if (empty()) return nullptr;
	auto i=begin();
	if (cmdpath.empty()) return i->second;
	++i;
	auto j = cmdpath.begin();
	while(i != end()) {
		if (i->first->name() != *j) return nullptr;
		++j;
		if (j == cmdpath.end()) break;
		++i;
	}
	if (i != end()) {
		return i->second;
	}
	return nullptr;
}

void c::pass1(param_path& v) {
	for (auto& i: *this) {
		i.second->pass1(v);
	}
}

bool c::exec(istream& is) {
	param_path v;
	auto r = fillv(v, is);
	if (!r) {
		return false;
	}
	const_cast<c*>(this)->pass1(v); /// rewrite v and let specializations have a first look.
	r = exec(v, v.begin());
	for (auto& i: v) delete i.second;
	return r;
}

c* c::lookup(const string& cmd) const {
	for (auto i = begin(); i != end(); ++i) {
		if (i->first.name == cmd) return i->second;
	}
	return nullptr;
}

c* c::root() {
	auto p=this;
	while(p->parent != nullptr) p = p->parent;
	return p;
}

const c* c::root() const {
	auto p=this;
	while(p->parent != nullptr) p = p->parent;
	return p;
}

bool c::ignore(char flag) const {
	if (ignoreFlags.find(flag) != ignoreFlags.end()) {
		return true;
	}
	if (parent == nullptr) {
		return false;
	}
	return parent->ignore(flag);
}

c* c::lookup(const param_path& v, param_path::const_iterator t) {
	assert(t != v.end());
	if (v.empty()) return nullptr;
	auto s = root();
	auto i = v.begin();
	assert(i->first->isRoot());
	if (v.size() == 1) return s;
	while(i != t) {
		++i;
		s = i->first;
		if (s == nullptr) return nullptr;
	}
	return s;
}

const c* c::lookup(const param_path& v, param_path::const_iterator t) const {
	assert(t != v.end());
	if (v.empty()) return nullptr;
	auto s = root();
	auto i = v.begin();
	assert(i->first->isRoot());
	if (v.size() == 1) return s;
	while(i != t) {
		++i;
		s = i->first;
		if (s == nullptr) return nullptr;
	}
	return s;
}

void c::print_error(const string& msg) {
	auto& os = *c::pos;
	os << "ERR " << msg << "\n\n";
}

//-------------------------------------------------params

params::params(b&& fd): b(fd) {
	assert(check_unique());
}

params::params(const params& fd, istream& is): b(fd) {
	assert(check_unique());
	while (is.good()) {
		string flag;
		auto g = is.tellg();
		is >> flag;
		if (is.fail()) break;
		if (flag.empty()) break;
		if (flag[0]!='-') {
			is.seekg(g, is.beg);
			break;
		}
		auto f = lookup(flag);
		if (f == nullptr) {
			ko = string("Unrecognized flag '") + flag + "'. ";
			break;
		}
		if (f->req_input) {
			is >> f->value;
			if (is.fail() || f->value.empty()) {
				ko = string("Expected value after '") + flag + "'. ";
				break;
			}
		}
		f->overriden=true;
	}
	assert(check_unique());
}

bool params::check_req(ostream& os) const {
	bool r = true;
	for (auto& i: *this) {
		if (!i.optional) {
			if (!i.overriden) {
				os << string("'") + i.name + "' is a required input. ";
				r=false;
			}
		}
	}
	return r;
}

bool params::check_unique() const {
	set<char> s;
	int n = 0;
	for (auto& i: *this) {
		s.emplace(i.short_name);
		++n;
		if (s.size() != n) {
			//let's do it again verbose
			s.clear();
			n = 0;
			for (auto& i: *this) {
				s.emplace(i.short_name);
				++n;
				if (s.size() != n) {
					cerr << "Error: " << i.short_name << " --" << i.name << " flag already defined.\n";
					assert(false);
					break;
				}
				cerr << "Ok: " << i.short_name << " --" << i.name << " flag defined at i.\n";
			}
		}
	}
	return s.size() == size();
}

void params::dump(const string& pfx, ostream& os) const {
	os << '\n';
	os << pfx << "flags:\n";
	string prefix = pfx + "  ";
	for (auto& i: *this)
		i.dump(prefix, os);
}

void params::dump_values(const string& pfx, ostream& os) const {
	for (auto& i: *this) {
		i.dump_valuex(pfx, os);
		os << '\n';
	}
}

bool params::any_set() const {
	for (auto& i: *this) {
		if (i.overriden || !i.value.empty() || !i.optional) {
			return true;
		}
	}
	return false;
}

bool params::any_set(const section& sec) const {
	for (auto& i: *this) {
		if (i.overriden || !i.value.empty() || !i.optional) {
			//if (!sec.ignore(i.short_name)) {
				return true;
			//}
		}
	}
	return false;
}

void params::dump_set(const string& pfx, ostream& os) const {
	for (auto& i: *this) {
		if (i.overriden || !i.value.empty() || !i.optional) {
			i.dump_valuex(pfx, os);
			os << '\n';
		}
	}
}

void params::dump_set(const string& pfx, const section& sec, ostream& os) const {
	for (auto& i: *this) {
//cout << ";:::: " << i.name << endl;
		if (i.overriden || !i.value.empty() || !i.optional) {
			i.dump_valuex(pfx, os);
			if (sec.ignore(i.short_name)) {
				os << " (Ignored)";
			}
			os << '\n';
		}
	}
}

flagdef* params::lookup(const string& f) {
	for (auto i = begin(); i != end(); ++i) {
		if (*i==f) return &*i;
	}
	return nullptr;
}

flagdef* params::lookup(char f) {
	for (auto i = begin(); i != end(); ++i) {
		if (i->short_name == f) return &*i;
	}
	return nullptr;
}

const string& params::get(const string& k) const {
	for (auto& i:*this) {
		if (i.name==k) {
			return i.value;
		}
	}
	throw "KO 90291 key doesn't exist.";
}

const string& params::get(char k) const {
	for (auto& i: *this) {
		if (i.short_name == k) return i.value;
	}
	throw "KO 90292 key doesn't exist.";
}

bool params::is_set(char flag) const {
	for (auto& i: *this) {
		if (i.short_name != flag) continue;
		return i.overriden;
	}
	return false;
}

bool params::is_set(const string& flag) const {
	for (auto& i: *this) {
		if (i.name != flag) continue;
		return i.overriden;
	}
	return false;
}

bool params::has(char flag) const {
	for (auto& i: *this) {
		if (i.short_name == flag) return true;
	}
	return false;
}

void params::set_optional(char flag) {
	for (auto& i: *this) {
		if (i.short_name != flag) continue;
		i.optional = true;
		break;
	}
}

void params::set_mandatory(char flag) {
	for (auto& i:* this) {
		if (i.short_name != flag) continue;
		i.optional = false;
		break;
	}
}

//-------------------------------------------------flagdef

void flagdef::dump(const string& pfx, ostream& os0) const {
	ostringstream os;
	if (short_name != ' ') {
		os << '-' << short_name;
		if (!name.empty()) os << ',';
		os << ' ';
	}
	if (!name.empty()) {
		os << "--" << name << ' ';
	}
	if (req_input) {
		if (value.empty()) {
			os << "<value> ";
		}
		else {
			os << "(value='" << value << "') ";
		}
	}
	else {
		if (overriden) {
			os << "(flag is set) ";
		}
	}
	os0 << pfx << fmt_field(os.str(), tabpos);
	if (!optional) {
		os0 << "(Required";
		if (overriden) {
			os0 << "-Ok";
		}
		os0 << ") ";
	}
	os0 << desc << '\n';
}

void flagdef::dump_valuex(const string& pfx, ostream& os) const {
	os << pfx << "--" << name << " = " << (req_input ? value : (overriden ? "(set)" : "(unset)") );
}

bool flagdef::operator == (const string& i) const {
	if (i.size() < 2) return false;
	if (i[0] != '-') return false;
	if (i[1] != '-') {
		return (string("-") + short_name)==i;
	}
	return (string("--") + name) == i;
}

bool flagdef::operator == (const flagdef& other) const {
	return short_name == other.short_name && name == other.name;
}

//-------------------------------------------------cmddef

bool cmddef::operator == (const string& i) const {
	return name == i;
}

bool cmddef::operator == (const cmddef& other) const {
	return name == other.name;
}


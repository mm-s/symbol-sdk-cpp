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

#include "DigitalAssets.h"
#include "catapult/Hashes.h"
#include <fstream>

namespace symbol { namespace core {
	/// Implementation for the class c 
	using c = DigitalAssets;
	using std::move;
	using std::ios;
	using std::exception;
	using std::ifstream;
	using std::ostringstream;
	using std::make_pair;
	using Hash = c::Hash;
	
	c::DigitalAssets() {
	}

	Hash c::digest(const vector<unsigned char>& buf) {
		catapult::RawBuffer b(buf);
		Hash h;
		catapult::crypto::Ripemd160(b, h);
		return move(h);
	}

	ko readFileX(const string& filename, vector<unsigned char>& buf) {
		ifstream is(filename, ios::binary | ios::ate);
		if (is.fail()) {
			return "KO 84010 does the file exist?.";
		}
		ifstream::pos_type pos = is.tellg();
		try {
			buf.resize(pos);
		}
		catch(exception e) {
			return "KO 84032 ";
		}
		is.seekg(0, ios::beg);
		is.read((char*)buf.data(), pos);
		return is.fail() ? "KO 93200 Stream failure." : ok;
	}

	pair<ko, Hash> c::digestFile(const string& file) {
		vector<unsigned char> buf;
		auto r = readFileX(file, buf);
		if (is_ko(r)) {
			return make_pair(r, Hash());
		}
		return make_pair(ok, digest(buf));
	}

	void c::toStream(ostream& os) const {
	}

}} // Namespace symbol::core

std::ostream& operator << (std::ostream& os, const symbol::core::DigitalAssets& o) {
	o.toStream(os);
	return os;
}


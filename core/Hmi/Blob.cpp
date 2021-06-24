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
#include "Blob.h"
#include "Network.h"

namespace symbol { namespace core { namespace hmi {
	
	using c = hmi::Blob;

	c::Params c::defParams() {
		return Params{
			{Blob_Flag, Blob_Name, true, true, Blob_Default, Blob_Desc},
		};
	}

	c::Blob(): b(defParams()) {
	}

	c::Blob(Params&&p): b(move(defParams())) {
		add(move(p));
	}

	c::~Blob() {
	}

	void c::help_flag(const FlagDef& f, ostream& os) const {
		if (f.short_name == Blob_Flag) {
			os << "Input an Hex string representing a memory representation of a Symbol object (Transaction, Block, other)\n";
			os << "Example: --blob B000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000009398F182AE4B7A91B1348D607BF6F0B01D62A0F9BA1965AC19EFA190BF3A0529000000000178544100000000000000000000000000000000781DECC1FAD08371CC057F4194697CE92D2838E20FDD67E8000001000000000000000000000000000100000000000000\n";
			return;
		}
		b::help_flag(f, os);
	}

	bool c::main(Params& p, ostream& os) {
		if (!b::main(p, os)) return false;
		m_blobOverriden = p.is_set(Blob_Flag);
		if (!m_blobOverriden) return true;
		
		{//Store the blob
			auto r = symbol::core::Network::decodeBlob(p.get(Blob_Flag));
			if (is_ko(r.first)) {
				os << r.first;
				return false;
			}
			m_blob = move(r.second);
			assert(!m_blob.empty());
		}

		{//Set Network identifier from Blob
			auto nid = core::Network::identifier(m_blob);
			auto r = setNetworkIdentifier(p, nid);
			if (is_ko(r)) {
				os << r;
				return false;
			}
		}

		{ //is Transaction
			if (core::Transaction::isTransaction(m_blob)) {
				if (core::Transaction::isTransferTransaction(m_blob)) {
					os << "Blob is a Transfer Transaction\n";
				}
				else {
					os << "The blob is an invalid transaction.\n";
					return false;
				}
			}
			else {
				os << "The blob is invalid.\n";
				return false;
			}
		}
		return true;
	}

	void c::init(const string& nm, const string& dc) {
		b::init(nm, dc);
	}

	bool c::pass1(ParamPath& v, ostream& os) {
//		cout << "CORE TRANSFER pass1 - call base" << endl;
		if (!b::pass1(v, os)) return false;
//		cout << "CORE TRANSFER pass1" << endl;
		//"tx", "transfer"  when the user runs this sequence reconfigure flag definitions (e.g. making some of them required)
		auto p=v.lookup({}); //TODO replace strings with their section name var
		assert(p!=nullptr);
		if(p->is_set(Blob::Blob_Flag)) {
//			auto p=v.lookup({Keys::Main_Command}); //TODO replace strings with their section name var
//			if (p!=nullptr) {
				disable(Keys::Main_Command);
//				os << "Keys functions are disabled when working with a blob."; 
//				return false;
//			}
		}
		return true;
	}

}}}


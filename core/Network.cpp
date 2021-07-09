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

#include "Network.h"
#include "catapult/Address.h"
#include "Transaction.h"
#include "Transaction/Transfer.h"
#include "catapult/EntityType.h"
#include "catapult/EntityIoUtils.h"
#include "catapult/TransferEntityType.h"
#include "catapult/SizeChecker.h"
#include "catapult/BufferInputStreamAdapter.h"

namespace symbol { namespace core {
	/// Implementation for the class c 
	using c = Network;
	using std::move;
	using std::ostringstream;
	using std::make_pair;

	c::Network(const Identifier& t): m_identifier(t) {
	}

	c::Network(const string& s) {
		m_identifier = identifier(s);
/*
		if (isValidIdentifier()) return;
		uint8_t x;
		istringstream is(s);
		is >> x;
		if (is.fail()) {
			return;
		}
		m_identifier = x;
*/		
	}

	c::~Network() {
	}

	string c::parse(const string& input, ptr<PublicKey>& pk, ptr<UnresolvedAddress>& addr) const {
		return const_cast<c&>(*this).parse(input, pk, addr, true);
	}

	string c::parse(const string& input, ptr<PublicKey>& pk, ptr<UnresolvedAddress>& addr, bool as_const) {
		bool spk{false}, sdd{false}, sad{false};
		spk = Keys::isKey(input);
		if (!spk) {
			sdd=UnresolvedAddress::isAddress(input);
			if (!sdd) {
				sad=true;
			}
		}
		bool fail = false;
		if (sdd || sad) {
			Identifier t=sdd ? identifierFromAddressHex(input) : identifierFromAccount(input);
			if (m_identifier != t) {
				if (as_const) {
					fail = true;
				}
				else {
					if (!isValid(t)) {
						fail = true;
					}
					else {
						m_identifier = t;
					}
				}
			}
		}
		if (fail) {
			ostringstream os;
			os << (sdd ? "Address" : "Account") << " is invalid for network type '" << identifierStr() << "'.";
			return os.str();
		}
		delete pk;
		delete addr;
		if (spk) {
			pk = core::Keys::createPublicKey(input);
			addr = newAddress(*pk);
		}
		else {
			pk = nullptr;
			addr = (sad ? newAccount(input) : newAddress(input));
		}
		if (!addr->isValid()) {
			ostringstream os;
			os << "Invalid input";
			delete pk;
			pk = nullptr;
			delete addr;
			addr = nullptr;
			return os.str();
		}
		return "";
	}

	string c::identifierDecHex() const {
		ostringstream os;
		os << dec << +(uint8_t)m_identifier << " (0x" << hex << +(uint8_t)m_identifier << ')';
		return os.str();
	}
	
	string c::identifierHex() const {
		ostringstream os;
		os << catapult::utils::HexFormat(m_identifier);
		return os.str();
	}

	c::Identifier c::identifier(uint8_t v) {
		Identifier x = static_cast<Identifier>(v);
		return x;
	}

	c::Identifier c::identifier(const string& s) {
		Identifier x;
		if (!catapult::model::TryParseValue(s, x)) {
			x = Identifier::Zero;
		}
		if (!listed(x)) {
			return Identifier::Zero;
		}
		return x;
	}

	c::Identifier c::identifierFromAddressHex(const string& hex) {
		catapult::Address x;
		if (!catapult::utils::TryParseHexStringIntoContainer(hex.data(), hex.size(), x)) {
			return Identifier::Zero;
		}
		auto r=static_cast<Identifier>(x[0]);
		if (!listed(r)) {
			return Identifier::Zero;
		}
		return r;
	}

	c::Identifier c::identifierFromAccount(const string& enc) {
		catapult::Address x;
		if (!catapult::model::TryParseValue(enc, x)) {
			return Identifier::Zero;
		}
		auto r=static_cast<Identifier>(x[0]);
		if (!listed(r)) {
			return Identifier::Zero;
		}
		return r;
	}

	c::Identifier c::identifier(const Blob& blob) {
		auto is = catapult::io::BufferInputStreamAdapter(blob);
		std::unique_ptr<catapult::model::VerifiableEntity> ptx;
		try {
			ptx = catapult::io::ReadEntity<catapult::model::VerifiableEntity>(is);
			//assert(ptx->Type == catapult::model::Entity_Type_Transfer);
		}
		catch (...) {
			///CATAPULT_THROW_INVALID_ARGUMENT("size is insufficient");
			return Identifier::Zero;
		}
		return ptx->Network;
		
	}

	string c::nodesUrl() const {
		using namespace catapult::model;
		if (m_identifier == Identifier::Public_Test) {
			return "http://explorer.testnet.symboldev.network:4001/nodes";
		}
		if (m_identifier == Identifier::Public) {
			return "http://explorer.symbolblockchain.io:4001/nodes";
		}
		return "";
	}

	void c::list(ostream& os) {
		os << "public public-test  private private-test  mijin mijin-test";
	}
	
	void c::list(const string& prefix, ostream& os) {
		os << prefix << "public (" << dec << +(uint8_t)Identifier::Public << ", 0x" << hex << +(uint8_t)Identifier::Public << ")" << '\n';
		os << prefix << "public-test (" << dec << +(uint8_t)Identifier::Public_Test << ", 0x" << hex << +(uint8_t)Identifier::Public_Test << ")" << '\n';
		os << prefix << "private (" << dec << +(uint8_t)Identifier::Private << ", 0x" << hex << +(uint8_t)Identifier::Private << ")" << '\n';
		os << prefix << "private-test (" << dec << +(uint8_t)Identifier::Private_Test << ", 0x" << hex << +(uint8_t)Identifier::Private_Test << ")" << '\n';
		os << prefix << "mijin (" << dec << +(uint8_t)Identifier::Mijin << ", 0x" << hex << +(uint8_t)Identifier::Mijin << ")" << '\n';
		os << prefix << "mijin-test (" << dec << +(uint8_t)Identifier::Mijin_Test << ", 0x" << hex << +(uint8_t)Identifier::Mijin_Test << ")" << '\n';
	}

	bool c::listed(const Identifier& t) {
		//using namespace catapult::model;
		switch(t) {
			case Identifier::Zero: return true;
			case Identifier::Public: return true;
			case Identifier::Private: return true;
			case Identifier::Mijin: return true;
			case Identifier::Public_Test: return true;
			case Identifier::Private_Test: return true;
			case Identifier::Mijin_Test: return true;
		}
		return false;
	}

	const char* c::identifierStr() const {
		return identifierStr(m_identifier);
	}

	const char* c::identifierStr(Identifier t) {
		using namespace catapult::model;
		switch(t) {
			case Identifier::Zero: return "zero";
			case Identifier::Public: return "public";
			case Identifier::Private: return "private";
			case Identifier::Mijin: return "mijin";
			case Identifier::Public_Test: return "public-test";
			case Identifier::Private_Test: return "private-test";
			case Identifier::Mijin_Test: return "mijin-test";
		}
		return "";
	}

	bool c::isValid(const UnresolvedAddress& a) const {
		return catapult::model::IsValidAddress(resolver().resolve(a), m_identifier);
	}

	bool c::isValidIdentifier() const {
		return isValid(m_identifier);
	}

	bool c::isValid(const Identifier& t) {
		return t!=Identifier::Zero && listed(t);
	}

	void c::toStream(ostream& os) const {
		using namespace catapult::utils;
		os << "type: " << identifierStr() << " seed: " << m_seed << '\n';
	}

	ptr<c::UnresolvedAddress> c::newAddress(const PublicKey& pubk) const {
		return new UnresolvedAddress(catapult::model::PublicKeyToAddress(pubk, m_identifier).copyTo<catapult::UnresolvedAddress>(), *this);
	}

	ptr<c::UnresolvedAddress> c::newAddress() const {
		return new UnresolvedAddress(*this);
	}

	ptr<c::UnresolvedAddress> c::newAccount(const string& acc) const {
		catapult::Address a;
		if (!catapult::model::TryParseValue(acc, a)) {
			return nullptr;
		}
		return new UnresolvedAddress(a.copyTo<catapult::UnresolvedAddress>(), *this);
	}

	ptr<c::UnresolvedAddress> c::newAddress(const string& hex) const {
		auto x=new UnresolvedAddress(*this);
		if (!catapult::utils::TryParseHexStringIntoContainer(hex.data(), hex.size(), *x)) {
			delete x;
			return nullptr;
		}
		return x;
	}

	bool c::setSeed(const string& hex) {
		if (!TryParseValue(hex, m_seed)) {
			return false;
		}
		return true;
	}

	pair<ko, c::Blob> c::decodeBlob(const string& hex) {
		Blob blob;
		if (hex.size()&1) {
			return make_pair("KO 44932 malformed hex string.", blob);
		}
		blob.resize(hex.size()/2);
		//Get from the bytes the instance type.
		if (!catapult::utils::TryParseHexStringIntoContainer(hex.data(), hex.size(), blob)) {
			return make_pair("KO 50948 Cannot decode hex.", move(blob));
		}
		return make_pair(ok, move(blob));
	}

	pair<ko, ptr<Transaction>> c::createTransaction(const Blob& mem) const {
		return Transaction::create(*this, mem);
	}

	pair<ko, ptr<Transfer>> c::createTransfer(const UnresolvedAddress& rcpt, const MosaicValues& m, const Amount& f, const TimeSpan& d, const vector<uint8_t>& msg, const ptr<PrivateKey>& encryptPrivateKey, const ptr<PublicKey>& encryptPublicKey) const {
		return Transfer::create(*this, rcpt, m, f, d, msg, encryptPrivateKey, encryptPublicKey);
	}

}} // Namespaces

std::ostream& operator << (std::ostream& os, const symbol::core::Network& o) {
	o.toStream(os);
	return os;
}




//pair<ko, ptr<catapult::model::Transfer>> c::createTransfer(const vector<uint8_t>& mem) {
	

	
	
	/*
	static_assert(std::is_trivially_copyable<catapult::model::Transaction>::value);
	catapult::model::Transaction* disguised=reinterpret_cast<catapult::model::Transaction*>(mem.data());
	catapult::model::Transaction* tx=new catapult::model::Transaction(*disguised);

	string instance_type;
	instance_type = "transfer";
	if ( instance_type == "transfer" ) {
		return new Transfer(tx);
	}
	delete tx;
	*/
//	return make_pair(ok, new Transfer(n, tx));
//}




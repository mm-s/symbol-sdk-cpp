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

namespace symbol { namespace core {
	/// Implementation for the class c 
	using c = Network;
	using std::move;
	using std::ostringstream;
	using std::make_pair;

	c::Network(const Identifier& t): m_identifier(t) {
	}

	c::Network(const string& s) {
		m_identifier=identifier(s);
	}

	c::~Network() {
	}

	string c::parse(const string& input, ptr<PublicKey>& pk, ptr<UnresolvedAddress>& addr) const {
		return const_cast<c&>(*this).parse(input, pk, addr, true);
	}

	string c::parse(const string& input, ptr<PublicKey>& pk, ptr<UnresolvedAddress>& addr, bool as_const) {
		bool spk{false}, sdd{false}, sad{false};
		spk=Keys::isKey(input);
		if (!spk) {
			sdd=UnresolvedAddress::isAddress(input);
			if (!sdd) {
				sad=true;
			}
		}
		bool fail=false;
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
			pk=nullptr;
			addr=(sad ? newAccount(input) : newAddress(input));
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

	string c::identifierHex() const {
		ostringstream os;
		os << catapult::utils::HexFormat(m_identifier);
		return os.str();
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

	bool c::listed(const Identifier& t) {
		using namespace catapult::model;
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

	ptr<symbol::core::Transfer> c::createTransfer(const UnresolvedAddress& rcpt, const Amount& a, const MosaicId& m, const Amount& f, const TimeSpan& d) {
		return Transfer::create(*this, rcpt, a, m, f, d);
	}

}} // Namespaces

std::ostream& operator << (std::ostream& os, const symbol::core::Network& o) {
	o.toStream(os);
	return os;
}


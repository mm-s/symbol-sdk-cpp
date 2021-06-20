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
#include "Transfer.h"
#include "../Network.h"
#include "../Transaction.h"
#include "../../Transaction/Transfer.h"
#include "../Keys.h"
//#include "../catapult/TransferTransaction.h"

namespace symbol { namespace core { namespace hmi {
	using c = core::hmi::Transfer; /// Implementation for class c 

	c::Params c::defParams() {
		return Params{
				{Recipient_Flag, Recipient_Name, false, true, Recipient_Default, Recipient_Desc},
				{Amount_Flag, Amount_Name, false, true, Amount_Default, Amount_Desc},
				{Mosaic_Flag, Mosaic_Name, false, true, Mosaic_Default, Mosaic_Desc},
				{Maxfee_Flag, Maxfee_Name, false, true, Maxfee_Default, Maxfee_Desc},
				{Deadline_Flag, Deadline_Name, false, true, Deadline_Default, Deadline_Desc},
				{Keys::Privkey_Flag, Keys::Privkey_Name, true, true, Keys::Privkey_Default, Keys::Privkey_Desc},
			};
	}
		
	c::Transfer(): b(defParams()) {
	}

	c::Transfer(Params&&p): b(move(p)) {
		add(defParams());
	}

	c::~Transfer() {
		delete m_tx;
	}

	Transaction* c::root() {
		return dynamic_cast<Transaction*>(b::root());
	}

	bool c::txTransfer(Params& p, ostream& os) {
		ptr<symbol::core::Transfer> tx;
		if (root()->blobOverriden()) {
			if (!symbol::core::Transaction::isTransferTransaction(root()->blob())) {
				os << "Blob is not a transfer transaction.";
				return false;
			}
			auto r = root()->network().createTransfer(root()->blob());
			if (is_ko(r.first)) {
				assert(r.second==nullptr);
				os << r.first;
				return false;
			}
			tx=r.second;
		}
		else {
			Amount am;
			if (!core::Transaction::parse(p.get(Amount_Flag), am)) {
				os << "Invalid amount.";
				return false;
			}
			if (am.unwrap()==0) {
				os << "Invalid amount.";
				return false;
			}

			MosaicId mo;
			if (!core::Transaction::parse(p.get(Mosaic_Flag), mo)) {
				os << "Invalid mosaic.";
				return false;
			}

			Amount maxfee;
			if (!core::Transaction::parse(p.get(Maxfee_Flag), maxfee)) {
				os << "Invalid maxfee.";
				return false;
			}

			TimeSpan deadline;
			if (!core::Transaction::parse(p.get(Deadline_Flag), deadline)) {
				os << "Invalid deadline.";
				return false;
			}

			vector<uint8_t> msg;
			if (p.is_set(Message_Flag)) {
				string s = p.get(Message_Flag);
				msg = vector<uint8_t>(s.begin(), s.end());
			}

			ptr<PublicKey> pk{nullptr};
			ptr<UnresolvedAddress> addr{nullptr};
			string e = root()->network().parse(p.get(Recipient_Flag), pk, addr, root()->networkOverriden());
			if ( !e.empty() ) {
				os << e;
				return false;
			}
			delete pk;

			auto r = root()->network().createTransfer(*addr, am, mo, maxfee, deadline, msg);
			delete addr;
			if (is_ko(r.first)) {
				assert(r.second==nullptr);
				os << r.first;
				return false;
			}
			tx = r.second;
		}
		
		ptr<PrivateKey> sk{nullptr};
		if (p.is_set(Keys::Privkey_Flag)) {
			sk = core::Keys::createPrivateKey( p.get(Keys::Privkey_Flag) );
			if (sk == nullptr) {
				os << "Input is not a private key.";
				return false;
			}
			if ( !tx->sign(*sk) ) {
				delete sk;
				delete tx;
				os << "Couldn't sign transaction with private key.";
				return false;
			}
			delete sk;
		}
		os << *tx << '\n';
		delete tx;
		return true;
	}

	void c::pass1(ParamPath& v) {
		b::pass1(v);
		//"tx", "transfer"  when the user runs this sequence reconfigure flag definitions (e.g. making some of them required)
		{
			auto p=v.lookup({}); //TODO replace strings with their section name var
			assert(p!=nullptr);
			if(p->is_set(Network::Blob_Flag)) {
				p->set_optional(Network::Seed_Flag);
				{
					auto p=v.lookup({Transaction::TX_Command, Transaction::Transfer_Command}); //TODO replace strings with their section name var
					if(p!=nullptr) {
						p->set_optional(Recipient_Flag);
						p->set_optional(Amount_Flag);
						p->set_optional(Mosaic_Flag);
						p->set_optional(Maxfee_Flag);
						p->set_optional(Deadline_Flag);
					}
				}
			}
		}
	}

	void c::init(const string& name, const string& desc) {
		b::init(name, desc);
		set_handler([&](Params& p, ostream& os) -> bool { return txTransfer(p, os); });
	}

}}}


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
#include "Transaction.h"
#include "Network.h"
#include "../Transaction.h"
#include "../catapult/TransferTransaction.h"

namespace symbol { namespace core { namespace hmi {
	using c = core::hmi::Transaction; /// Implementation for class c 

	c::~Transaction() {
		delete m_tx;
	}

	bool c::txTransfer(const Params& p, ostream& os) {
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
		string e = network().parse(p.get(Recipient_Flag), pk, addr, networkOverriden());
		if ( !e.empty() ) {
			os << e;
			return false;
		}
		delete pk;

		ptr<Transfer> tx = network().createTransfer(*addr, am, mo, maxfee, deadline, msg);
		delete addr;
		if (tx == nullptr) {
			os << "Invalid transfer.";
			return false;
		}
		
		ptr<PrivateKey> sk{nullptr};
		if (p.is_set(Privkey_Flag)) {
			sk = core::Keys::createPrivateKey( p.get(Privkey_Flag) );
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
			auto p=v.lookup({Transaction::TX_Command, Transaction::Transfer_Command}); //TODO replace strings with their section name var
			if (p!=nullptr) {
	//cout << "KKKKKKKK" << endl;
				if(p->is_set(Mem_Flag)) {
	//cout << "AAAAAAAAAA" << endl;
					p->set_optional(Recipient_Flag);
					p->set_optional(Amount_Flag);
					p->set_optional(Mosaic_Flag);
					p->set_optional(Maxfee_Flag);
					p->set_optional(Deadline_Flag);
					{
						auto p=v.lookup({}); //TODO replace strings with their section name var
						if (p!=nullptr) {
							p->set_optional(Network::Seed_Flag);
						}
					}

				}
	//cout << "XXXXXXXXXXXXXXXXXX" << endl;
			}
		}
	}

	bool c::tx(const Params& p, ostream& os) {
		if (p.is_set(Mem_Flag)) {
			auto r = core::Transaction::create(p.get(Mem_Flag));
			if (is_ko(r.first)) {
				os << r.first;
				return false;
			}
			assert(m_tx == nullptr);
			m_tx = r.second;
		}
		return true;
	}

	ptr<c::Section> c::createSectionTxTransfer() {
		auto s=new Section(Params{
			{Recipient_Flag, Recipient_Name, false, true, Recipient_Default, Recipient_Desc},
			{Amount_Flag, Amount_Name, false, true, Amount_Default, Amount_Desc},
			{Mosaic_Flag, Mosaic_Name, false, true, Mosaic_Default, Mosaic_Desc},
			{Maxfee_Flag, Maxfee_Name, false, true, Maxfee_Default, Maxfee_Desc},
			{Deadline_Flag, Deadline_Name, false, true, Deadline_Default, Deadline_Desc},
			{Privkey_Flag, Privkey_Name, true, true, Privkey_Default, Privkey_Desc},
			{Mem_Flag, Mem_Name, true, true, Mem_Default, Mem_Desc},
		});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return txTransfer(p, os); });
		return s;
	}

	ptr<c::Section> c::createSectionTx() {
		auto s=new Section(Params{});
		s->add(CmdDef{Transfer_Command, Transfer_Command_Desc}, createSectionTxTransfer());
		s->set_handler([&](const Params& p, ostream& os) -> bool { return tx(p, os); });
		return s;
	}

	void c::init(const string& name, const string& desc) {
		b::init(name, desc);
		add(CmdDef{TX_Command, TX_Command_Desc}, createSectionTx());
	}

}}}


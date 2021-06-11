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
#include "../Transaction.h"
#include "../catapult/TransferTransaction.h"

namespace symbol { namespace core { namespace hmi {
	using c = hmi::Transaction; /// Implementation for class c 

	void c::init(const string& name, const string& desc) {
		b::init(name, desc);
		add(CmdDef{"tx", "Operations with transactions."}, createSectionTx());
	}

	bool c::txTransfer(const Params& p, ostream& os) {
		symbol::Amount am;
		if (!symbol::Transaction::parse(p.get(Amount_Flag), am)) {
		    os << "Invalid amount.";
		    return false;
		}
		if (am.unwrap()==0) {
		    os << "Invalid amount.";
		    return false;
		}

		symbol::MosaicId mo;
		if (!symbol::Transaction::parse(p.get(Mosaic_Flag), mo)) {
		    os << "Invalid mosaic.";
		    return false;
		}

		symbol::Amount maxfee;
		if (!symbol::Transaction::parse(p.get(Maxfee_Flag), maxfee)) {
		    os << "Invalid maxfee.";
		    return false;
		}

		symbol::TimeSpan deadline;
		if (!symbol::Transaction::parse(p.get(Deadline_Flag), deadline)) {
		    os << "Invalid deadline.";
		    return false;
		}
		
		ptr<PublicKey> pk{nullptr};
		ptr<UnresolvedAddress> addr{nullptr};
		string e = network().parse(p.get(Recipient_Flag), pk, addr, networkOverriden());
		if ( !e.empty() ) {
		    os << e;
		    return false;
		}
		delete pk;

		ptr<Transfer> tx = network().createTransfer(*addr, am, mo, maxfee, deadline);
		delete addr;
		if (tx == nullptr) {
		    os << "Invalid transfer.";
		    return false;
		}
		
		ptr<PrivateKey> sk{nullptr};
		if (p.is_set(Privkey_Flag)) {
		    sk=symbol::Keys::createPrivateKey( p.get(Privkey_Flag) );
		    if (sk==nullptr) {
		        os << "Input is not a private key.";
		        return false;
		    }
		    if (!tx->sign(*sk)) {
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

	ptr<c::Section> c::createSectionTxTransfer() {
		auto s=new Section(Params{
		    {Recipient_Flag, "recipient", false, true, "", "Recipient address."},
		    {Amount_Flag, "amount", false, true, "0", "Amount of mosaics."},
		    {Mosaic_Flag, "mosaic", false, true, "", "mosaic id."},
		    {Maxfee_Flag, "maxfee", false, true, "", "Max fee."},
		    {Deadline_Flag, "deadline", false, true, "", "Deadline ms."},
		    {Privkey_Flag, "private-key", true, true, "", "Sign using the supplied private key in HEX format."},
		});
		s->set_handler([&](const Params& p, ostream& os) -> bool { return txTransfer(p, os); });
		return s;
	}

	ptr<c::Section> c::createSectionTx() {
		auto s=new Section(Params{});
		s->add(CmdDef{"transfer", "Transfer transaction."}, createSectionTxTransfer());
		return s;
	}

}}}

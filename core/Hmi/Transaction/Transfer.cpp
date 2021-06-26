/*
 * Copyright 2021 NEM
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
				{Message_Flag, Message_Name, true, true, Message_Default, Message_Desc},
				{Encrypt_Flag, Encrypt_Name, true, false, Encrypt_Default, Encrypt_Desc},
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
		delete m_sk;
	}

	Transaction* c::root() {
		return dynamic_cast<Transaction*>(b::root());
	}

	void c::help_flag(const FlagDef& f, ostream& os) const {
		if (f.short_name == Mosaic_Flag) {
			os << "Value in Hex format starting with 0x\n";
			return;
		}
		b::help_flag(f, os);
	}

	bool c::main(Params& p, bool last, ostream& os) {
//cout << "core txTransfer" << endl;
		if (p.is_set(Keys::Privkey_Flag)) {
			m_sk = core::Keys::createPrivateKey( p.get(Keys::Privkey_Flag) );
			if (m_sk == nullptr) {
				os << "Input is not a private key.";
				return false;
			}
		}
		
		if (root()->blobOverriden()) {
			if (!symbol::core::Transaction::isTransferTransaction(root()->blob())) {
				os << "Blob is not a transfer transaction.";
				return false;
			}
			auto r = root()->network().createTransaction(root()->blob());
			if (is_ko(r.first)) {
				assert(r.second==nullptr);
				os << r.first;
				return false;
			}
			assert(r.second!=nullptr);
			delete m_tx;
			m_tx = dynamic_cast<core::Transfer*>(r.second);
			if (m_tx==nullptr) {
				delete r.second;
				os << "Unexpected error.";
				return false;
			}
			
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

			bool encrypMsg = p.is_set(Encrypt_Flag);

			ptr<PublicKey> pk{nullptr};
			ptr<UnresolvedAddress> addr{nullptr};
			string e = root()->network().parse(p.get(Recipient_Flag), pk, addr, root()->networkOverriden());
			if ( !e.empty() ) {
				os << e;
				return false;
			}

			if (encrypMsg && (m_sk==nullptr || pk==nullptr)) {
				os << "Cannot encrypt message.";
				if (m_sk==nullptr) os << " Signer private key is required.";
				if (pk==nullptr) os << " Recipient public key is required.";
				return false;
			}

			auto r = root()->network().createTransfer(*addr, am, mo, maxfee, deadline, msg, encrypMsg?m_sk:nullptr, pk);
			delete pk;
			delete addr;
			if (is_ko(r.first)) {
				assert(r.second==nullptr);
				os << r.first;
				return false;
			}
			delete m_tx;
			m_tx = r.second;
		}
		assert(m_tx!=nullptr);
		
		if (last) os << *m_tx << '\n';
		return true;
	}

	bool c::cmdSign(Params& p, bool last, ostream& os) {
		if (!m_sk) {
			os << "Private key is required.";
			return false;
		}
		if (!m_tx) {
			os << "Tx is required.";
			return false;
		}
		auto r=m_tx->sign(*m_sk);
		if (is_ko(r)) {
			os << r;
			return false;
		}
		os << *m_tx << '\n';
		return true;
	}

	ptr<c::Section> c::createSectionSign() {
		auto s=new Section(Params{});
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return cmdSign(p, last, os); });
		return s;
	}

	void c::init(const string& name, const string& desc) {
		b::init(name, desc);
		add(CmdDef{Sign_Command, Sign_Command_Desc}, createSectionSign());
		set_handler([&](Params& p, bool last, ostream& os) -> bool { return main(p, last, os); });
	}

	bool c::pass1(ParamPath& v, ostream& os) {
//		cout << "CORE TRANSFER pass1 - call base" << endl;
		if (!b::pass1(v, os)) return false;
//		cout << "CORE TRANSFER pass1" << endl;
		//"tx", "transfer"  when the user runs this sequence reconfigure flag definitions (e.g. making some of them required)
		{
			auto p=v.lookup({});
			assert(p!=nullptr);
			if(p->is_set(Blob::Blob_Flag)) {
				p->set_optional(Network::Seed_Flag);
				{
					auto p=v.lookup({Transaction::Main_Command, Transaction::Transfer_Command});
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
		{
			{
				auto p=v.lookup({Transaction::Main_Command, Transaction::Transfer_Command});
				if(p!=nullptr) {
					if(p->is_set(Encrypt_Flag)) {
						p->set_mandatory(Keys::Privkey_Flag);
					}
				}
			}
			{
				auto p=v.lookup({Transaction::Main_Command, Transaction::Transfer_Command, Sign_Command});
				if(p!=nullptr) {
					p->set_mandatory(Keys::Privkey_Flag);
				}
			}
		}
		return true;
	}

}}}


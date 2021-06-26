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
		if (f.short_name == Recipient_Flag) {
			os << "Recipient. Either:\n";
			os << " - A public Key: binary_fixed(32 bytes). Example: 8750670A44B937B57AECC706A73F734915D8A317BA83C10C58E584039BC938FB. (network type agnostic).\n";
			os << " - An address: binary_fixed(24 bytes). Example: 9879AE2D834B2336F8586443D68735B21914936660BB4A13 (Includes the network type in the encoding).\n";
			os << " - An account: Example: TB424LMDJMRTN6CYMRB5NBZVWIMRJE3GMC5UUEY (Exact format depends on the network type).\n";
			return;
		}
		if (f.short_name == Amount_Flag) {
			os << "Amount (uint64). Number of mosaics to transfer. Decimal value >=0. Example: 1234.\n";
			os << "Separate with comma multiple amounts corresponding to multiple mosaics. Example: 123,456,789\n";
			return;
		}
		if (f.short_name == Mosaic_Flag) {
			os << "UnresolvedMosaicId (uint64). Value in Hex format starting with 0x\n";
			os << "Separate with comma multiple mosaics corresponding to multiple amounts. Example: 0xA1A2A3A4A5A6A7A8,0xB1B2B3B4B5B6B7B8,0xC1C2C3C4C5C6C7C8\n";
			return;
		}
		if (f.short_name == Deadline_Flag) {
			os << "Timestamp (uint64). Time elapsed since the creation of the nemesis block.\n";
			os << "If a transaction does not get included in a block before the deadline is reached, it is deleted.\n";
			os << "Deadlines are only allowed to lie up to 24 hours ahead.\n";
			os << "Example of equivalent values: 3600000ms, 3600s, 60m, 1h.\n";
			return;
		}
		if (f.short_name == Maxfee_Flag) {
			os << "Amount (uint64). Maximum fee allowed to spend for the transaction.\n";
			return;
		}
		if (f.short_name == Keys::Privkey_Flag) {
			os << "Key. binary_fixed(32 bytes). Example: 3E8B6A5DDADFD3EAE9359C1FAEC17CCA5B6676D06EA625F0DF1393575488DF10.\n";
			return;
		}
		b::help_flag(f, os);
	}

	namespace {
		template<typename T>
		ko parse(std::string input, vector<T>& v, function<bool(const string&, T&)> p) {
			int s{0};
			for (auto& ch:input) {
				if (ch==',') { ch=' '; ++s; }
			}
			v.reserve(s+1);
			istringstream is(input);
			while(is.good()) {
				string a;
				is >> a;
				T r;
				if (!p(a, r)) {
					return "KO 66987";
				}
				v.emplace_back(move(r));
			}
			return ok;
		}
		
		using MosaicValues = symbol::core::Transaction::MosaicValues;
		pair<ko, MosaicValues> amounts(const vector<MosaicId>& m, const vector<Amount>& a) {
			MosaicValues r;
			if (m.empty()) return make_pair("KO 83352 At least one mosaic is required.", r);
			if (a.empty()) return make_pair("KO 83353 At least one amount is required.", r);
			if (a.size()!=m.size()) return make_pair("KO 83354 Number of amounts must match the number of mosaics.", r);
			set<MosaicId> u;
			for (auto&i:m) {
				u.emplace(i);
			}
			if (u.size()!=m.size()) {
				 return make_pair("KO 83355 Mosaics must be different.", r);
			}
			r.reserve(m.size());
			auto im=m.begin();
			auto ia=a.begin();
			while(im!=m.end()) {
				r.emplace_back(make_pair(*im, *ia));
				++im;
				++ia;
			}
			return make_pair(ok, move(r));
		}
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
			using MosaicValues = symbol::core::Transaction::MosaicValues;
			MosaicValues m;
			{
				vector<Amount> am;
				{
					auto r=parse<Amount>(p.get(Amount_Flag), am, [](const string& i, Amount& v)->bool { return core::Transaction::parse(i, v); });
					if (is_ko(r)) {
						os << r;
						return false;
					}
				}
				/*
				if (am.unwrap()==0) {
					os << "Invalid amount.";
					return false;
				}
	*/
				vector<MosaicId> mo;
				{
					auto r=parse<MosaicId>(p.get(Mosaic_Flag), mo, [](const string& i, MosaicId& v)->bool { return core::Transaction::parse(i, v); });
					if (is_ko(r)) {
						os << r;
						return false;
					}
				}

				{
					auto r=amounts(mo, am);
					if (is_ko(r.first)) {
						os << r.first;
						return false;
					}
					m=move(r.second);
				}
			}
			assert(!m.empty());

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

			auto r = root()->network().createTransfer(*addr, m, maxfee, deadline, msg, encrypMsg?m_sk:nullptr, pk);
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


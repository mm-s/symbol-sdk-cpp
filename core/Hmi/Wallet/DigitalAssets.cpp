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
#include "../Network.h"
#include "../../DigitalAssets.h"
#include "../../catapult/Logging.h"
#include "../Keys.h"
#include "../Wallet.h"

namespace symbol { namespace core { namespace hmi {
	using c = hmi::DigitalAssets; /// Implementation for class c 
	using symbol::ko;

	c::~DigitalAssets() {
		delete m_privateKey;
	}

	bool c::handlerSign(Params& p, bool last, ostream& os) {
		assert ( m_privateKey != nullptr );
		os << *m_privateKey << "\n";
		return false;
	}

	bool c::handlerDigest(Params& p, bool last, ostream& os) {
		pair<ko, symbol::core::Hash> r = core::DigitalAssets::digestFile(m_file);
		if (is_ko(r.first)) {
			os << r.first;
			return false;
		}
		os << r.second << endl;
		return true;
	}

	ptr<c::Section> c::createSectionSign() {
		auto s=new Section(Params{});
		/// Add a handler for command sign
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return handlerSign(p, last, os); });
		return s;
	}

	ptr<c::Section> c::createSectionDigest() {
		auto s=new Section(Params{});
		/// Add a handler for command sign
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return handlerDigest(p, last, os); });
		s->ignoreFlags.insert(Keys::Privkey_Flag);
		return s;
	}

	c::FlagDef c::flagdefFile() {
		return FlagDef{File_Flag, "file", false, true, "", "Select file."};
	}

	bool c::handlerDA(Params& p, bool last, ostream& os) {
		assert(p.is_set(File_Flag));
		m_file = p.get(File_Flag);
		
		assert(m_privateKey == nullptr);
		if(p.is_set(Keys::Privkey_Flag)) {
			m_privateKey = core::Keys::createPrivateKey( p.get(Keys::Privkey_Flag) );
			if (m_privateKey == nullptr) {
				os << "Input is not a private key.";
				return false;
			}
		}
		return true;
	}

	ptr<c::Section> c::createSectionDA() {
		auto s = new Section(Params{flagdefFile(), Keys::flagdefPrivkey(false)});
		s->add(CmdDef{Sign_Command, Sign_Command_Desc}, createSectionSign());
		s->add(CmdDef{Digest_Command, Digest_Command_Desc}, createSectionDigest());
		/// Add more commands here.
		/// Add a handler for this command.
		s->set_handler([&](Params& p, bool last, ostream& os) -> bool { return handlerDA(p, last, os); });
		/// s->set_handler([&](const Params& p, ostream& os) -> bool { return you_handler_function(p, os); });
		s->ignoreFlags.insert(Network::Network_Flag);
		s->ignoreFlags.insert(Network::Seed_Flag);
		return s;
	}

	void c::init(const string& name, const string& desc) {
		//CATAPULT_LOG(trace) << "Test Log";
		b::init(name, desc);
		add(CmdDef{DA_Command, DA_Command_Desc}, createSectionDA());
		/// Add more commands here.
		/// Add a handler for this command.
	}

	bool c::pass1(ParamPath& v, ostream& os) {
	//cout << "pass1" << endl;
		if (!b::pass1(v, os)) return false;
		auto p=v.lookup({Wallet::Command, DA_Command, Sign_Command});
		if (p!=nullptr) {
			auto r=v.lookup({Wallet::Command});
			assert(r->has(Keys::Privkey_Flag));
			r->set_mandatory(Keys::Privkey_Flag);
		}
		return true;
	}

}}}


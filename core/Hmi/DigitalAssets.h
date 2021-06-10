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
#pragma once

#include "Transaction.h"

namespace symbol { namespace core { namespace hmi {

	/// Human-Machine Interface. NFT section command processor (offline)
	class DigitalAssets: public hmi::Transaction {
		/// Base class b.
		using b = hmi::Transaction;

	protected:
		/// Flags, Options
		static constexpr auto File_Flag{'f'};

	public:	
		/// constructors, initialization, destructor
		using b::Transaction;
		~DigitalAssets();

		/// Provide the program name and a description.
		void init(const string& name, const string& desc) override;

	private:
		/// menu command: tx
		ptr<Section> createSectionMain();
		ptr<Section> createSectionSign();
		ptr<Section> createSectionDigest();
		static FlagDef flagdefFile();

	protected: /// menu command: tx transfer
		bool handlerMain(const Params& p, ostream&);
		bool handlerSign(const Params&, ostream&); /// Command Handler
		bool handlerDigest(const Params&, ostream&); /// Command Handler
		void pass1(ParamPath& v) override;

	private:
		string m_file;
		ptr<PrivateKey> m_privateKey{nullptr};

	};

}}}


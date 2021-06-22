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
#include "Keys.h"
#include "../Network.h"

namespace symbol { namespace core { namespace hmi {

	/// Human-Machine Interface (HMI). Main-menu command processor (offline)
	class Blob: public hmi::Keys {
		/// Base class b
		using b = hmi::Keys;
	public:
		static constexpr auto Blob_Flag{'B'};
		static constexpr auto Blob_Name{"blob"};
		static constexpr auto Blob_Default{""};
		static constexpr auto Blob_Desc{"Memory representation. Hex string."};

	public:
		using CoreBlob = symbol::core::Network::Blob;

	public:
		/// Construction, initialization, destruction
		Blob();
		Blob(Params&&);
		~Blob() override;

		void init(const string& name, const string& desc) override;

	public: // Options related to Network.
		/// The Network instance the user wants to use.
		inline const CoreBlob& blob() const { return m_blob; }
		inline bool blobOverriden() const { return m_blobOverriden; }

	public:
		/// Gettrs & Setters
		/// User selectd JSON output
		void help_flag(const FlagDef&, ostream&) const override;

	public: /// Generic
		/// String converter
		//bool mainHandler(Params&, ostream&) override;
		bool main(Params&, ostream&) override;

	protected:
		/// Handler for empty command

		ptr<Section> createSectionBlobInfo();
		bool blobInfo(Params&, ostream&);

	private:

		static Params defParams();

	private:
		/// Output preferences
		bool m_blobOverriden{false};
		CoreBlob m_blob;
	};

}}}


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
#include "Main.h"

namespace symbol { namespace core { namespace hmi {

	/// Human-Machine Interface (HMI). Main-menu command processor (offline)
	class Network: public hmi::Main {
		/// Base class b
		using b = hmi::Main;
	public:
		static constexpr auto Network_Flag{'n'};
		static constexpr auto Seed_Flag{'s'};
		
		static constexpr auto Blob_Flag{'B'};
		static constexpr auto Blob_Name{"blob"};
		static constexpr auto Blob_Default{""};
		static constexpr auto Blob_Desc{"Memory representation in Hex format."};

	public:
		using Blob = symbol::core::Network::Blob;

	public:
		/// Construction, initialization, destruction
		Network();
		Network(Params&&);
		~Network() override;

		void init(const string& name, const string& desc) override;

	public: //hmi
		///Opportunity to rewrite Params before command execution.
		void pass1(ParamPath&) override;

	public: // Options related to Network.
		/// The Network instance the user wants to use.
		inline const core::Network& network() const { return *m_network; }
		inline core::Network& network() { return *m_network; }

		/// Tells whether the user has explicitely set the network identifier.
		inline const string& home() const { return m_home; }
		inline bool networkOverriden() const { return m_networkOverriden; }
		ko setNetworkIdentifier(Params& p, const core::Network::Identifier&);

		inline const Blob& blob() const { return m_blob; }
		inline bool blobOverriden() const { return m_blobOverriden; }

	public:
		/// Gettrs & Setters
		/// User selectd JSON output
		inline bool json() const { return m_json; }

		/// User selected to hide labels in text output
		inline bool hideLabels() const { return m_hideLabels; }


	public: /// Generic
		/// String converter
		template<typename T>
		static string toString(const T& o) {
			ostringstream os;
			os << o;
			return os.str();
		}

	protected:
		/// Handler for empty command
		virtual bool mainHandler(Params&, ostream&);

	private:
		/// Flag configuration for initalizing the section
		static FlagDef flagdefHome();
		static FlagDef flagdefVerbose();
		static FlagDef flagdefNetwork();
		static FlagDef flagdefOutput();
		static FlagDef flagdefHideLabels();
		static FlagDef flagdefSeed();
		static FlagDef flagdefBlob();

	private:
		/// Output preferences
		bool m_json{false};
		bool m_hideLabels{false};

		/// Network instance
		ptr<core::Network> m_network{nullptr};
		bool m_networkOverriden{false};
		string m_home;

		bool m_blobOverriden{false};
		symbol::core::Network::Blob m_blob;
	};

	/// String converter. Type with special treatment required in order to be printed as a number, not as a char.
	template<> string Main::toString(const uint8_t& o);

}}}


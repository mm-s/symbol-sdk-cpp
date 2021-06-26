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
		static constexpr auto Network_Name{"network"};
		static constexpr auto Network_Default{"public-test"};
		static constexpr auto Network_Desc{"Network type."};
 		
		static constexpr auto Seed_Flag{'s'};
		static constexpr auto Seed_Name{"seed"};
		static constexpr auto Seed_Default{""};
		static constexpr auto Seed_Desc{"Network generation hash seed."};

	public:
		/// Construction, initialization, destruction
		Network();
		Network(Params&&);
		~Network() override;

		void init(const string& name, const string& desc) override;

	public: // Options related to Network.
		/// The Network instance the user wants to use.
		inline const core::Network& network() const { return *m_network; }
		inline core::Network& network() { return *m_network; }

		/// Tells whether the user has explicitely set the network identifier.
		inline const string& home() const { return m_home; }
		inline bool networkOverriden() const { return m_networkOverriden; }
		ko setNetworkIdentifier(Params& p, const core::Network::Identifier&);

	public:
		/// Gettrs & Setters
		/// User selectd JSON output
		inline bool json() const { return m_json; }

		/// User selected to hide labels in text output
		inline bool hideLabels() const { return m_hideLabels; }

		void help_flag(const FlagDef&, ostream&) const override;

	public: /// Generic
		/// String converter
		template<typename T>
		static string toString(const T& o) {
			ostringstream os;
			os << o;
			return os.str();
		}

		bool main(Params&, bool last, ostream&) override;

	protected:
		/// Handler for empty command
		//bool mainHandler(Params&, ostream&) override;


	private:

		/// Flag configuration for initalizing the section
/*
		static FlagDef flagdefNetwork();
		static FlagDef flagdefHome();
		static FlagDef flagdefVerbose();
		static FlagDef flagdefOutput();
		static FlagDef flagdefHideLabels();
		static FlagDef flagdefSeed();
		static FlagDef flagdefBlob();
*/

		static Params defParams();

	private:
		/// Output preferences
		bool m_json{false};
		bool m_hideLabels{false};

		/// Network instance
		ptr<core::Network> m_network{nullptr};
		bool m_networkOverriden{false};
		string m_home;

	};

	/// String converter. Type with special treatment required in order to be printed as a number, not as a char.
//	template<> string Main::toString(const uint8_t& o);

}}}


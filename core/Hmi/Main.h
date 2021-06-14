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
#include "../base.h"
#include "../Network.h"
#include "../conch.h"
#include <sstream>

namespace symbol { namespace core { namespace hmi {

	/// Human-Machine Interface (HMI). Main-menu command processor (offline)
	class Main: public conch::section {
		/// Base class b
		using b = conch::section;
		
	public:
		using Section = conch::section;
		using Params = conch::params;
		using CmdDef = conch::cmddef;
		using FlagDef = conch::flagdef;
		using ParamPath = b::param_path;

	public:
		/// Flags and Options
		static constexpr auto Home_Flag{'H'};
		static constexpr auto Verbose_Flag{'v'};
		static constexpr auto Output_Flag{'o'};
		static constexpr auto HideLabels_Flag{'w'};
		static constexpr auto Network_Flag{'n'};
		static constexpr auto Seed_Flag{'s'};

	public:
		/// Construction, initialization, destruction
		Main();
		Main(Params&&);
		~Main() override;

		void init(const string& name, const string& desc) override;

	public: //hmi
 		///Opportunity to rewrite Params before command execution.
 		void pass1(ParamPath&) override;

		/// Print key-value on output stream.
		void kv_text(const vector<pair<string, string>>&, ostream&);
		void kv_json(const vector<pair<string, string>>&, ostream&);

	public: // Options related to Network.
		/// The Network instance the user wants to use.
		inline const symbol::Network& network() const { return *m_network; }
		inline symbol::Network& network() { return *m_network; }

		/// Tells whether the user has explicitely set the network identifier.
		inline bool networkOverriden() const { return m_networkOverriden; }
		inline const string& home() const { return m_home; }
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
		virtual bool mainHandler(const Params& p, ostream& os);

	private:
		/// Flag configuration for initalizing the section
		static FlagDef flagdefHome();
		static FlagDef flagdefVerbose();
		static FlagDef flagdefNetwork();
		static FlagDef flagdefOutput();
		static FlagDef flagdefHideLabels();
		static FlagDef flagdefSeed();

	private:
		/// Output preferences
		bool m_json{false};
		bool m_hideLabels{false};

		/// Network instance
		ptr<symbol::Network> m_network{nullptr};
		bool m_networkOverriden{false};
		string m_home;
	};

	/// String converter. Type with special treatment required in order to be printed as a number, not as a char.
	template<> string Main::toString(const uint8_t& o);

}}}


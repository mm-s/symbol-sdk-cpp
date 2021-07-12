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
#include "conch.h"
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
		static constexpr auto Home_Name{"home"};
		static constexpr auto Home_Default{"~/.symbol"};
		static constexpr auto Home_Desc{"Home directory."};

		static constexpr auto Verbose_Flag{'v'};
		static constexpr auto Verbose_Name{"verbose"};
		static constexpr auto Verbose_Default{""};
		static constexpr auto Verbose_Desc{"Print extra information about what the program is doing."};

		static constexpr auto Output_Flag{'o'};
		static constexpr auto Output_Name{"output"};
		static constexpr auto Output_Default{"text"};
		static constexpr auto Output_Desc{"Output format. -h for list of valid options."};

		struct Output {
			enum Format {
				text, etext, json, ejson, bin, hex,
			};
			static pair<ko, Output::Format> from_string(const string&);
		};

	public:
		/// Construction, initialization, destruction
		Main();
		Main(Params&&);
		~Main() override;

		void init(const string& name, const string& desc) override;

	public: //hmi

		/// Print key-value on output stream.
		//void kv_text(const vector<pair<string, string>>&, ostream&);
		//void kv_json(const vector<pair<string, string>>&, ostream&);

	public: // Options related to this app.
		inline const string& home() const { return m_home; }

	public:
		/// Gettrs & Setters
		/// User selectd JSON output
//		inline bool json() const { return m_json; }
//		inline bool compact() const { return m_compact; }
		inline Output::Format output() const { return m_output; }

		/// User selected to hide labels in text output
//		inline bool hideLabels() const { return m_hideLabels; }


	public: /// Generic
		/// String converter
		template<typename T>
		static string toString(const T& o) {
			ostringstream os;
			os << o;
			return os.str();
		}

		static string toHex(const vector<uint8_t>&);

		virtual bool main(Params&, bool last, ostream&);
		void help_flag(const FlagDef&, ostream&) const override;

		template<typename dto>
		void print(const dto& o, ostream& os) const {
			switch (m_output) {
				case Output::json:
					o.toJson(false, os);
					break;
				case Output::ejson:
					o.toJson(true, os);
					break;
				case Output::text:
					o.toText("", false, "", os);
					break;
				case Output::etext:
					o.toText("", true, "", os);
					break;
				case Output::bin: {
					auto v=o.toBin();
					os.write(reinterpret_cast<const char*>(v.data()), v.size());
					}
					break;
				case Output::hex:
					os << toHex(o.toBin());
					break;
			}
		}
	protected:
		/// Handler for empty command
//		virtual bool mainHandler(Params& p, ostream& os);

	private:
		/// Flag configuration for initalizing the section
		static Params defParams();

	private:
		/// Output preferences
		Output::Format m_output;
//		bool m_json{false};
//		bool m_compact{false};
//		bool m_hideLabels{false};

		/// Network instance
		string m_home;
	};

	/// String converter. Type with special treatment required in order to be printed as a number, not as a char.
	template<> string Main::toString(const uint8_t& o);

}}}


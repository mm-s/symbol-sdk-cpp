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
	class Utils: public hmi::Main {
		/// Base class b
		using b = hmi::Main;
	public:
		static constexpr const char* Main_Command = "utils";
		static constexpr const char* Main_Command_Desc = "Generic utilily functions.";

		static constexpr const char* Hex_Command = "hex";
		static constexpr const char* Hex_Command_Desc = "Hex encoder/decoder.";

		static constexpr const char* Decode_Command = "decode";
		static constexpr const char* Decode_Command_Desc = "decode Hex data.";

		static constexpr const char* Encode_Command = "encode";
		static constexpr const char* Encode_Command_Desc = "encode Hex data.";

		/// Flags and Options
		static constexpr char Data_Flag{'d'};
		static constexpr auto Data_Name{"data"};
		static constexpr auto Data_Default{""};
		static constexpr auto Data_Desc{"Input data."};

	public:
		/// Construction, initialization, destruction
		using b::Main;

		void init(const string& name, const string& desc) override;

	public: /// related options.

	public:
		/// Gettrs & Setters
		/// User selectd JSON output
		void help_flag(const FlagDef&, ostream&) const override;

	public: /// Generic
		bool cmdMain(Params&, bool last, ostream&);
		bool cmdHex(Params&, bool last, ostream&);
		bool cmdHexDecode(Params&, bool last, ostream&);

		bool pass1(ParamPath&, ostream&) override;

	protected:
		ptr<Section> createSectionHexDecode();
		ptr<Section> createSectionHex();
		ptr<Section> createSectionMain();

	private:
		string m_data;
		static Params defParams();
	};

}}}


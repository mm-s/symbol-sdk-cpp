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

#include <symbol/core/Hmi.h>
#include "dto/dto.h"

namespace symbol {

	/// Human-Machine Interface. Fetch REST API section command processor (online+offline)
	class HmiFetch: public core::Hmi {
		using b = core::Hmi;
		
		static constexpr char url_flag{'r'};
		static constexpr char acc_flag{'a'};
		#ifdef DEBUG
			static constexpr auto Def_Url="http://nem.mm-studios.com:3000";
		#else
			static constexpr auto Def_Url="http://127.0.0.1:3000";
		#endif
	public:
	
		HmiFetch();

		section* create_section_fetch();
		bool fetch(const Params&, ostream&);

		section* create_section_fetch_node();
		bool fetch_node(const Params&, ostream&);

		section* create_section_fetch_nodes();
		bool fetch_nodes(const Params&, ostream&);

		section* create_section_fetch_node_health();
		bool fetch_node_health(const Params&, ostream&);

		section* create_section_fetch_node_info();
		bool fetch_node_info(const Params&, ostream&);

		section* create_section_fetch_node_peers();
		bool fetch_node_peers(const Params&, ostream&);

		section* create_section_fetch_chain();
		bool fetch_chain(const Params&, ostream&);

		section* create_section_fetch_account();
		bool fetch_account(const Params&, ostream&);


		void init(const string& name, const string& desc) override;

		inline const string& getUrl() const { return url; }
	protected:
		bool main_handler(const Params&, ostream&) override;

	private:
		static FlagDef flagdef_url();
		   
	private:
		string url;

	};

}


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

#include "base.h"
#include "dto/dto.h"

namespace symbol {

	/// Human-Machine Interface. Fetch REST API section command processor (online+offline)
	class HmiFetch: public core::Hmi {
		/// Base class b.
		using b = core::Hmi;

	private:		
		static constexpr char Url_Flag{'r'};
		static constexpr char Acc_Flag{'a'};
		#ifdef DEBUG
			static constexpr auto Def_Url="http://nem.mm-studios.com:3000";
		#else
			static constexpr auto Def_Url="http://127.0.0.1:3000";
		#endif
	public:
		/// Constructor, Initialization	
		HmiFetch();
		void init(const string& name, const string& desc) override;

		/// menu command: fetch
		ptr<Section> createSectionFetch(); /// init
		bool fetch(const Params&, ostream&); /// Handler

		/// menu command: fetch node
		ptr<Section> createSectionFetchNode(); /// init
		bool fetchNode(const Params&, ostream&); /// Handler

		/// menu command: fetch nodes
		ptr<Section> createSectionFetchNodes(); /// init
		bool fetchNodes(const Params&, ostream&); /// Handler

		/// menu command: fetch node health
		ptr<Section> createSectionFetchNodeHealth(); /// init
		bool fetchNodeHealth(const Params&, ostream&); /// Handler

		/// menu command: fetch node info
		ptr<Section> createSectionFetchNodeInfo(); /// init
		bool fetchNodeInfo(const Params&, ostream&); /// Handler

		/// menu command: fetch node peers
		ptr<Section> createSectionFetchNodePeers(); /// init
		bool fetchNodePeers(const Params&, ostream&); /// Handler

		/// menu command: fetch chain
		ptr<Section> createSectionFetchChain(); /// init
		bool fetchChain(const Params&, ostream&); /// Handler

		/// menu command: fetch account
		ptr<Section> createSectionFetchAccount(); /// init
		bool fetchAccount(const Params&, ostream&); /// Handler

	public:
		/// Getters & Setters
		inline const string& getUrl() const { return m_url; }

	protected:
		bool mainHandler(const Params&, ostream&) override;

	private:
		static FlagDef flagdefUrl();
		   
	private:
		string m_url; /// REST endpoint

	};

}


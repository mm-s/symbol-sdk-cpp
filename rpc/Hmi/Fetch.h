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
#include "../dto/dto.h"
#include <symbol/core/Hmi.h>
#include <iostream>

namespace symbol { namespace hmi {

	/// Human-Machine Interface. Fetch REST API section command processor (online+offline)
	class Fetch: public symbol::core::Hmi {
		/// Base class b.
		using b = symbol::core::Hmi;

	public:
		using ostream = std::ostream;
		using b::Section;

	public:
		static constexpr auto Offline_Flag{'x'};
		static constexpr auto Offline_Name{"offline"};
		static constexpr auto Offline_Default{""};
		static constexpr auto Offline_Desc{"Work offline."};

		#ifndef NDEBUG
			static constexpr auto Def_Url="http://nem.mm-studios.com:3000";
		#else
			static constexpr auto Def_Url="http://127.0.0.1:3000";
		#endif

		static constexpr char Url_Flag{'r'};
		static constexpr auto Url_Name{"url"};
		static constexpr auto Url_Default{Def_Url};
		static constexpr auto Url_Desc{"API node URL."};

	public:
		/// Constructor, Initialization	
		Fetch();
		Fetch(Params&&p);
		
		void init(const string& name, const string& desc) override;

	private:
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
		inline const string& url() const { return m_url; }
		inline bool offline() const { return m_offline; }

	protected:
		bool mainHandler(Params&, ostream&) override;

		///Opportunity to rewrite Params before command execution.
		void pass1(ParamPath&) override;

	private:
//		static FlagDef flagdefOffline();
//		static FlagDef flagdefUrl();
		static Params defParams();

	private:
		string m_url; /// REST endpoint
		bool m_offline{false};

	};

}}


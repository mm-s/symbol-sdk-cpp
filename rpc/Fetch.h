#pragma once

#include "base.h"
#include <symbol/core/dto/Account.h>
#include <symbol/core/dto/ApiNode.h>
#include <symbol/core/dto/NetNodes.h>

namespace symbol {

	struct Fetch {
		static pair<ko, core::dto::Account> account(const string& url, const string& account);

		struct ApiNode {
			static pair<ko, core::dto::ApiNode::Info> info(const string& url);
			static pair<ko, core::dto::ApiNode::Peers> peers(const string& url);
			static pair<ko, core::dto::ApiNode::Health> health(const string& url);
		};

		static pair<ko, core::dto::NetNodes> netNodes(const string& url);

	};

} // namespaces


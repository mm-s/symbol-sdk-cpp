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
#include "Config.h"
#include <iostream>
#include <ctime>
#include "catapult/Logging.h"
#include "catapult/LoggingConfiguration.h"
#include "catapult/ConfigurationFileLoader.h"
#include "catapult/preprocessor.h"
#include <filesystem>

symbol::core::Config System;

namespace symbol { namespace core {
	using c = Config;


	namespace {
		using namespace catapult::config;
		using namespace catapult::utils;
		LoggingConfiguration LoadLoggingConfiguration(const std::string& userLoggingConfigurationPath) {
			// if the user has provided a path, try that first
			std::vector<std::filesystem::path> loggingConfigurationPaths;
			if (!userLoggingConfigurationPath.empty())
				loggingConfigurationPaths.emplace_back(userLoggingConfigurationPath);

			// fallback to searching in some default locations
			constexpr auto Default_Logging_Configuration_Filename = "config-logging.properties";
			loggingConfigurationPaths.emplace_back(Default_Logging_Configuration_Filename);
			loggingConfigurationPaths.emplace_back(std::filesystem::path("..") / "resources" / Default_Logging_Configuration_Filename);

			for (const auto& loggingConfigurationPath : loggingConfigurationPaths) {
				if (std::filesystem::exists(loggingConfigurationPath))
					return LoadIniConfiguration<LoggingConfiguration>(loggingConfigurationPath);
			}

			auto config = LoggingConfiguration::Uninitialized();
			config.Console.SinkType = LogSinkType::Sync;
			config.Console.Level = LogLevel::debug;
			return config;
		}

		std::unique_ptr<catapult::utils::LogFilter> CreateLogFilter(const catapult::config::BasicLoggerConfiguration& config) {
			auto pFilter = std::make_unique<LogFilter>(config.Level);
			for (const auto& pair : config.ComponentLevels)
				pFilter->setLevel(pair.first.c_str(), pair.second);

			return pFilter;
		}
	
		std::shared_ptr<void> SetupLogging(const catapult::config::LoggingConfiguration& config) {
			auto pBootstrapper = std::make_shared<catapult::utils::LoggingBootstrapper>();
			pBootstrapper->addConsoleLogger(catapult::config::GetConsoleLoggerOptions(config.Console), *CreateLogFilter(config.Console));
			return PORTABLE_MOVE(pBootstrapper);
		}

	}


	ko c::init(const string& home) {
		m_home = home;
		// 1. seed the random number generator
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		auto pLoggingGuard = SetupLogging(LoadLoggingConfiguration(m_home));
		return checkPlatform();
	}

	bool c::isBigEndian() {
		union {
			uint16_t i;
			char c[2];
		} bint = {0x0102};
		return bint.c[0] == 1;
	}

	ko c::checkPlatform() {
		if (isBigEndian()) {
			return "KO 71892 This program cannot run on big-endian systems";
		}
		return ok;
	}

}}


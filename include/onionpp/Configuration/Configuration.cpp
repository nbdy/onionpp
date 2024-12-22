//
// Created by nbdy on 03.12.24.
//

#include <utility>

#include "Configuration.h"

#include "onionpp/Option/OptionMapping.h"

#include <iostream>
#include <onionpp/onionpp.h>

void onionpp::Configuration::setDefaults() {
  for (const auto& mapping : OptionMapping) {
    if (mapping.DefaultValue.empty() == false) {
      m_OptionMap[mapping.ConfigOption] = mapping.DefaultValue;
    }
  }
}

void onionpp::Configuration::parseEnvironment() {
  for (const auto& mapping: OptionMapping) {
    if (const char* value = std::getenv(mapping.EnvVar.data())) {
      setOption(mapping.ConfigOption, value);
    }
  }
}

void onionpp::Configuration::parseArguments(const uint32_t argc, char** argv) {
  for (uint32_t idx = 0; idx < argc; idx++) {
    auto arg = std::string(argv[idx]);
    for (const auto& mapping : OptionMapping) {
      if (arg == mapping.ArgVar) {
        if (const auto valIdx = idx + 1; argc >= valIdx) {
          setOption(mapping.ConfigOption, argv[valIdx]);
        } else {
          std::cout << "No value specified for '" << arg << "'!" << std::endl;
        }
      }
    }
  }
}

onionpp::Configuration::Configuration() {
  setDefaults();
  parseEnvironment();
}

onionpp::Configuration::Configuration(const uint16_t i_Socks5Port) {
  setDefaults();
  parseEnvironment();
  m_OptionMap[Option::SOCKSPort] = std::to_string(i_Socks5Port);
}

onionpp::Configuration::Configuration(ConfigOptionMap i_ConfigOptionMap) : m_OptionMap(std::move(i_ConfigOptionMap)) {}

onionpp::Configuration::Configuration(const int argc, char** argv) {
  setDefaults();
  parseEnvironment();
  parseArguments(argc, argv);
}

onionpp::ConfigOptionMap onionpp::Configuration::getOptions() const {
  return m_OptionMap;
}

std::string onionpp::Configuration::getOption(const Option& i_Option) const {
  if (m_OptionMap.contains(i_Option)) {
    return m_OptionMap.at(i_Option);
  }
  return "";
}

void onionpp::Configuration::setOption(const Option& i_Option, const std::string& i_Value) {
  switch (i_Option) {
    case Option::ControlPassword:
      m_OptionMap[Option::HashedControlPassword] = hashPassword(i_Value);
      break;
    default:
      m_OptionMap[i_Option] = i_Value;
      break;
  }
}

std::string onionpp::Configuration::getSocks5Address() const {
  return "socks5h://127.0.0.1:" + getOption(Option::SOCKSPort);
}


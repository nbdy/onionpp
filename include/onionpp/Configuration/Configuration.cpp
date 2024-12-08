//
// Created by nbdy on 03.12.24.
//

#include "Configuration.h"

#include "onionpp/Option/OptionMapping.h"

void onionpp::Configuration::parseEnvironment() {
  for (const auto& mapping: OptionMapping) {
    if (const char* value = std::getenv(mapping.EnvVar)) {
      m_OptionMap[mapping.Option] = value;
    }
  }
}

onionpp::Configuration::Configuration() {
  parseEnvironment();
}

onionpp::Configuration::Configuration(const uint16_t i_Socks5Port) {
  parseEnvironment();
  m_OptionMap[Option::SOCKSPort] = std::to_string(i_Socks5Port);
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
  m_OptionMap[i_Option] = i_Value;
}

std::string onionpp::Configuration::getSocks5Address() const {
  return "socks5h://127.0.0.1:" + getOption(Option::SOCKSPort);
}


//
// Created by nbdy on 03.12.24.
//

#ifndef OPTIONMAPPING_H
#define OPTIONMAPPING_H

#include <array>

#include "Option.h"

namespace onionpp {
struct ConfigOptionMapping {
  Option m_Option;
  const char* m_EnvironmentVariable;
  const char* m_TorOption;
};

static constexpr std::array<ConfigOptionMapping, 23> getOptionMapping() {
  return {
    {
      {Option::SOCKSPort, "TOR_SOCKS_PORT", "--SOCKSPort"},
      {Option::DataDirectory, "TOR_DATA_DIRECTORY", "--DataDirectory"},
      {Option::ControlPort, "TOR_CONTROL_PORT", "--ControlPort"},
      {Option::HashedControlPassword, "TOR_HASHED_CONTROL_PASSWORD", "--HashedControlPassword"},
      {Option::CookieAuthentication, "TOR_COOKIE_AUTHENTICATION", "--CookieAuthentication"},
      {Option::HiddenServiceDir, "TOR_HIDDEN_SERVICE_DIR", "--HiddenServiceDir"},
      {Option::HiddenServicePort, "TOR_HIDDEN_SERVICE_PORT", "--HiddenServicePort"},
      {Option::ORPort, "TOR_ORPORT", "--ORPort"},
      {Option::Address, "TOR_ADDRESS", "--Address"},
      {Option::OutboundBindAddress, "TOR_OUTBOUND_BIND_ADDRESS", "--OutboundBindAddress"},
      {Option::Nickname, "TOR_NICKNAME", "--Nickname"},
      {Option::RelayBandwidthRate, "TOR_RELAY_BANDWIDTH_RATE", "--RelayBandwidthRate"},
      {Option::RelayBandwidthBurst, "TOR_RELAY_BANDWIDTH_BURST", "--RelayBandwidthBurst"},
      {Option::AccountingMax, "TOR_ACCOUNTING_MAX", "--AccountingMax"},
      {Option::AccountingRule, "TOR_ACCOUNTING_RULE", "--AccountingRule"},
      {Option::AccountingStart, "TOR_ACCOUNTING_START", "--AccountingStart"},
      {Option::ContactInfo, "TOR_CONTACT_INFO", "--ContactInfo"},
      {Option::DirPort, "TOR_DIR_PORT", "--DirPort"},
      {Option::DirPortFrontPage, "TOR_DIR_PORT_FRONT_PAGE", "--DirPortFrontPage"},
      {Option::MyFamily, "TOR_MY_FAMILY", "--MyFamily"},
      {Option::ExitPolicy, "TOR_EXIT_POLICY", "--ExitPolicy"},
      {Option::BridgeRelay, "TOR_BRIDGE_RELAY", "--BridgeRelay"},
      {Option::PublishServerDescriptor, "TOR_PUBLISH_SERVER_DESCRIPTOR", "--PublishServerDescriptor"}
    }
  };
}
static constexpr auto OptionMapping = getOptionMapping();
}

#endif //OPTIONMAPPING_H

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

static constexpr std::array<ConfigOptionMapping, 19> getOptionMapping() {
  return {{{Option::SocksPort, "TOR_SOCKS_PORT", "--SocksPort"},
           {Option::ControlPort, "TOR_CONTROL_PORT", "--ControlPort"},
           {Option::DataDirectory, "TOR_DATA_DIRECTORY", "--DataDirectory"},
           {Option::Log, "TOR_LOG", "--Log"},
           {Option::HiddenServiceDir, "TOR_HIDDEN_SERVICE_DIR", "--HiddenServiceDir"},
           {Option::HiddenServicePort, "TOR_HIDDEN_SERVICE_PORT", "--HiddenServicePort"},
           {Option::Bridge, "TOR_BRIDGE", "--Bridge"},
           {Option::ExitNodes, "TOR_EXIT_NODES", "--ExitNodes"},
           {Option::EntryNodes, "TOR_ENTRY_NODES", "--EntryNodes"},
           {Option::ExcludeNodes, "TOR_EXCLUDE_NODES", "--ExcludeNodes"},
           {Option::ClientOnly, "TOR_CLIENT_ONLY", "--ClientOnly"},
           {Option::RelayBandwidthRate, "TOR_RELAY_BANDWIDTH_RATE", "--RelayBandwidthRate"},
           {Option::RelayBandwidthBurst, "TOR_RELAY_BANDWIDTH_BURST", "--RelayBandwidthBurst"},
           {Option::ORPort, "TOR_OR_PORT", "--ORPort"},
           {Option::FallbackDir, "TOR_FALLBACK_DIR", "--FallbackDir"},
           {Option::DisableNetwork, "TOR_DISABLE_NETWORK", "--DisableNetwork"},
           {Option::ListenAddress, "TOR_LISTEN_ADDRESS", "--ListenAddress"},
           {Option::ControlPortHashedPassword, "TOR_CONTROL_PORT_HASHED_PASSWORD", "--ControlPortHashedPassword"},
           {Option::HashedControlPassword, "TOR_HASHED_CONTROL_PASSWORD", "--HashedControlPassword"}}};
}
static constexpr auto OptionMapping = getOptionMapping();
}

#endif //OPTIONMAPPING_H

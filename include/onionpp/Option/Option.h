//
// Created by nbdy on 03.12.24.
//

#ifndef CONFIGOPTION_H
#define CONFIGOPTION_H

#include <unordered_map>
#include <string>
#include <array>

namespace onionpp {
enum class Option {
  SocksPort,
  ControlPort,
  DataDirectory,
  Log,
  HiddenServiceDir,
  HiddenServicePort,
  Bridge,
  ExitNodes,
  EntryNodes,
  ExcludeNodes,
  ClientOnly,
  RelayBandwidthRate,
  RelayBandwidthBurst,
  ORPort,
  FallbackDir,
  DisableNetwork,
  ListenAddress,
  ControlPortHashedPassword,
  HashedControlPassword
};
using ConfigOptionMap = std::unordered_map<Option, std::string>;
};

#endif //CONFIGOPTION_H

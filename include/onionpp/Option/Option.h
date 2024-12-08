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
  SOCKSPort,
  DataDirectory,
  ControlPort,
  HashedControlPassword,
  CookieAuthentication,
  HiddenServiceDir,
  HiddenServicePort,
  ORPort,
  Address,
  OutboundBindAddress,
  Nickname,
  RelayBandwidthRate,
  RelayBandwidthBurst,
  AccountingMax,
  AccountingRule,
  AccountingStart,
  ContactInfo,
  DirPort,
  DirPortFrontPage,
  MyFamily,
  ExitPolicy,
  BridgeRelay,
  PublishServerDescriptor,
};
using ConfigOptionMap = std::unordered_map<Option, std::string>;
};

#endif //CONFIGOPTION_H

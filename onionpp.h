//
// Created by nbdy on 14.02.22.
//

#ifndef ONIONPP__ONIONPP_H_
#define ONIONPP__ONIONPP_H_

#include <cstdint>
#include <stddef.h>

extern "C" {
#ifdef _WIN32
typedef int SOCKET;
#endif

#include "feature/api/tor_api.h"

void crypto_rand(char*, int32_t);
void base16_encode(char *dest, size_t destlen, const char *src, size_t srclen);
void secret_to_key_rfc2440(
    char *key_out, size_t key_out_len, const char *secret,
    size_t secret_len, const char *s2k_specifier);
}

#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#ifdef _WIN32
#include "mingw.thread.h"
#else
#include <thread>
#endif

namespace onionpp {

// tor/src/app/config/config.c
class TorConfiguration {
  uint16_t m_u16SocksPort = 4269;

  bool m_bEnableControlPort = false;
  bool m_bHashedPasswordAuthentication = false;
  std::string m_sHashedControlPassword;
  uint16_t m_u16ControlPort = 42069;

  bool m_bEnableHiddenService = false;
  std::string m_sHiddenServiceDirectory;
  uint16_t m_u16HiddenServicePort = 6969;

 public:
  TorConfiguration() = default;
  explicit TorConfiguration(uint16_t i_u16SocksPort): m_u16SocksPort(i_u16SocksPort) {}

  ~TorConfiguration() = default;


  // setter

  void setSocksPort(uint16_t i_u16Value) {
    m_u16SocksPort = i_u16Value;
  }

  void setControlPortEnabled(bool i_bValue) {
    m_bEnableControlPort = i_bValue;
  }

  void setHashedPasswordAuthenticationEnabled(bool i_bValue) {
    m_bHashedPasswordAuthentication = i_bValue;
  }

  void setHashedControlPassword(const std::string& i_sValue) {
    m_sHashedControlPassword = i_sValue;
  }

  void setControlPort(uint16_t i_u16Value) {
    m_u16ControlPort = i_u16Value;
  }

  void setHiddenServiceEnabled(bool i_bValue) {
    m_bEnableHiddenService = i_bValue;
  }

  void setHiddenServiceDirectory(const std::string& i_sValue) {
    m_sHiddenServiceDirectory = i_sValue;
  }

  void setHiddenServicePort(uint16_t i_u16Value) {
    m_u16HiddenServicePort = i_u16Value;
  }

  // getter

  uint16_t getSocksPort() const {
    return m_u16SocksPort;
  }

  bool isControlPortEnabled() const {
    return m_bEnableControlPort;
  }

  bool isHashedPasswordAuthenticationEnabled() const {
    return m_bHashedPasswordAuthentication;
  }

  std::string getHashedControlPassword() {
    return m_sHashedControlPassword;
  }

  uint16_t getControlPort() const {
    return m_u16ControlPort;
  }

  bool isHiddenServiceEnabled() const {
    return m_bEnableHiddenService;
  }

  std::string getHiddenServiceDirectory() {
    return m_sHiddenServiceDirectory;
  }

  uint16_t getHiddenServicePort() const {
    return m_u16HiddenServicePort;
  }
};

class Tor {
  std::thread *m_Thread = nullptr;

  void startThread(const TorConfiguration& i_Configuration) {
    m_Thread = new std::thread([i_Configuration] { start(i_Configuration); });
  }

 public:
  Tor() { startThread(TorConfiguration()); }
  explicit Tor(const TorConfiguration& i_Configuration) { startThread(i_Configuration); }

  ~Tor() {
    if (m_Thread->joinable()) {
      m_Thread->join();
    }
    delete m_Thread;
  }

  static const char* getVersion() {
    return tor_api_get_provider_version();
  }

  static std::stringstream createArguments(TorConfiguration i_Configuration) {
    std::stringstream rVal;
    rVal << "tor";

    rVal << " SocksPort ";
    rVal << i_Configuration.getSocksPort();

    if(i_Configuration.isControlPortEnabled()) {
      rVal << " ControlPort ";
      rVal << i_Configuration.getControlPort();

      if(i_Configuration.isHashedPasswordAuthenticationEnabled()) {
        rVal << " HashedControlPassword ";
        rVal << i_Configuration.getHashedControlPassword();
      }
    }

    if(i_Configuration.isHiddenServiceEnabled()) {
      rVal << " HiddenServiceDir ";
      rVal << i_Configuration.getHiddenServiceDirectory();

      rVal << " HiddenServicePort ";
      rVal << i_Configuration.getHiddenServicePort();
    }

    return rVal;
  }

  static bool start(std::vector<char*> i_Arguments) {
    bool rVal = false;
    auto* cfg = tor_main_configuration_new();
    if (tor_main_configuration_set_command_line(cfg, static_cast<int32_t>(i_Arguments.size()), i_Arguments.data()) == 0) {
      rVal = tor_run_main(cfg) == 0;
    }
    tor_main_configuration_free(cfg);
    return rVal;
  }

  static bool start(const TorConfiguration& i_Configuration) {
    auto argumentLine = createArguments(i_Configuration);
    std::vector<char*> args {};
    std::string seg;
    while(std::getline(argumentLine, seg, ' ')) {
      auto *data = new char[seg.size() + 1];
      strcpy(data, seg.data());
      args.push_back(data);
    }
    auto rVal = start(args);
    for(auto* val : args) {
      delete val;
    }
    return rVal;
  }

  static std::string hashPassword(const std::string& i_sValue) {
    char output[256];
    memset(output, 0, sizeof(output));
    char key[29];
    crypto_rand(key, 8);
    key[8] = (uint8_t) 96;
    secret_to_key_rfc2440(key + 9, 20, &i_sValue[0], i_sValue.size(), key);
    base16_encode(output, sizeof(output), key, sizeof(key));
    return "16:" + std::string(output);
  }
};
}// namespace onionpp

#endif//ONIONPP__ONIONPP_H_

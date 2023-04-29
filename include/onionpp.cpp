//
// Created by nbdy on 10.04.23.
//

#include "onionpp.h"

#include <cstring>
#include <iostream>
#include <pthread.h>
#include <unistd.h>

extern "C" {
#ifdef _WIN32
typedef int SOCKET;
#endif

#include "feature/api/tor_api.h"

void crypto_rand(char *, int32_t);
void base16_encode(char *dest, size_t destlen, const char *src, size_t srclen);
void secret_to_key_rfc2440(
    char *key_out, size_t key_out_len, const char *secret,
    size_t secret_len, const char *s2k_specifier);

int control_get_bootstrap_percent(void);
void tor_shutdown_event_loop_and_exit(int exitcode);
}

void onionpp::TorConfiguration::setSocksPort(uint16_t i_u16Value) {
  m_u16SocksPort = i_u16Value;
}
void onionpp::TorConfiguration::setControlPortEnabled(bool i_bValue) {
  m_bEnableControlPort = i_bValue;
}
void onionpp::TorConfiguration::setHashedPasswordAuthenticationEnabled(bool i_bValue) {
  m_bHashedPasswordAuthentication = i_bValue;
}
void onionpp::TorConfiguration::setHashedControlPassword(const std::string &i_sValue) {
  m_sHashedControlPassword = i_sValue;
}
void onionpp::TorConfiguration::setControlPort(uint16_t i_u16Value) {
  m_u16ControlPort = i_u16Value;
}
void onionpp::TorConfiguration::setHiddenServiceEnabled(bool i_bValue) {
  m_bEnableHiddenService = i_bValue;
}
void onionpp::TorConfiguration::setHiddenServiceDirectory(const std::string &i_sValue) {
  m_sHiddenServiceDirectory = i_sValue;
}
void onionpp::TorConfiguration::setHiddenServicePort(uint16_t i_u16Value) {
  m_u16HiddenServicePort = i_u16Value;
}
std::vector<char *> onionpp::TorConfiguration::toArguments() {
  std::stringstream cliStream;
  cliStream << "tor";

  if (m_bQuiet) {
    cliStream << " --quiet ";
  }

  cliStream << " SocksPort ";
  cliStream << m_u16SocksPort;

  if (m_bEnableControlPort) {
    cliStream << " ControlPort ";
    cliStream << m_u16ControlPort;

    if (m_bHashedPasswordAuthentication) {
      cliStream << " HashedControlPassword ";
      cliStream << m_sHashedControlPassword;
    }
  }

  if (m_bEnableHiddenService) {
    cliStream << " HiddenServiceDir ";
    cliStream << m_sHiddenServiceDirectory;

    cliStream << " HiddenServicePort ";
    cliStream << m_u16HiddenServicePort;
  }

  std::vector<char *> rVal;
  std::string seg;
  while (std::getline(cliStream, seg, ' ')) {
    auto *data = new char[seg.size() + 1];
    strcpy(data, seg.data());
    rVal.push_back(data);
  }

  return rVal;
}
std::string onionpp::TorConfiguration::getProxyAddress() const {
  return "socks5h://127.0.0.1:" + std::to_string(m_u16SocksPort);
}
const char *onionpp::STor::getVersion() {
  return tor_api_get_provider_version();
}
bool onionpp::Tor::_start() {
  return _start(m_Configuration);
}
bool onionpp::Tor::_start(onionpp::TorConfiguration &i_Configuration) {
  return start(i_Configuration.toArguments());
}
bool onionpp::Tor::start(std::vector<char *> i_Arguments) {
  bool rVal = false;
  auto *cfg = tor_main_configuration_new();
  if (tor_main_configuration_set_command_line(cfg, static_cast<int32_t>(i_Arguments.size()), i_Arguments.data()) == 0) {
    rVal = tor_run_main(cfg) == 0;
  }
  for (auto *arg : i_Arguments) { free(arg); }
  tor_main_configuration_free(cfg);
  return rVal;
}
std::string onionpp::STor::hashPassword(const std::string &i_sValue) {
  char output[256];
  memset(output, 0, sizeof(output));
  char key[29];
  crypto_rand(key, 8);
  key[8] = (uint8_t) 96;
  secret_to_key_rfc2440(key + 9, 20, &i_sValue[0], i_sValue.size(), key);
  base16_encode(output, sizeof(output), key, sizeof(key));
  return "16:" + std::string(output);
}

bool onionpp::Tor::isBootstrapped() {
  return control_get_bootstrap_percent() == 100;
}
void onionpp::Tor::start(bool i_Wait) {
  pthread_create(&m_Thread, nullptr, &Tor::_start_thread, this);
  if (i_Wait) {waitUntilBootstrapped();}
}
void onionpp::Tor::waitUntilBootstrapped() {
  while(!isBootstrapped()) { sleep(1); }
}
onionpp::Tor::~Tor() {
  tor_shutdown_event_loop_and_exit(0);
  pthread_join(m_Thread, nullptr);
}
void *onionpp::Tor::_start_thread(void *i_Tor) {
  Tor *tor = static_cast<Tor*>(i_Tor);
  tor->_start();
  return nullptr;
}

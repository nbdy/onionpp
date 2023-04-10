#ifndef ONIONPP_ONIONPP_H
#define ONIONPP_ONIONPP_H

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>
#include <pthread.h>

#define NAMESPACE_NAME onionpp

namespace NAMESPACE_NAME {
class TorConfiguration {
  bool m_bQuiet = false;
  uint16_t m_u16SocksPort = 54269;

  bool m_bEnableControlPort = false;
  bool m_bHashedPasswordAuthentication = false;
  std::string m_sHashedControlPassword;
  uint16_t m_u16ControlPort = 42069;

  bool m_bEnableHiddenService = false;
  std::string m_sHiddenServiceDirectory;
  uint16_t m_u16HiddenServicePort = 56969;

 public:
  TorConfiguration() = default;
  explicit TorConfiguration(uint16_t i_u16SocksPort) : m_u16SocksPort(i_u16SocksPort) {}
  ~TorConfiguration() = default;

  // setter
  void setSocksPort(uint16_t i_u16Value);
  void setControlPortEnabled(bool i_bValue);
  void setHashedPasswordAuthenticationEnabled(bool i_bValue);
  void setHashedControlPassword(const std::string &i_sValue);
  void setControlPort(uint16_t i_u16Value);
  void setHiddenServiceEnabled(bool i_bValue);
  void setHiddenServiceDirectory(const std::string &i_sValue);
  void setHiddenServicePort(uint16_t i_u16Value);

  // getter
  [[nodiscard]] uint16_t getSocksPort() const { return m_u16SocksPort; }
  [[nodiscard]] bool isControlPortEnabled() const { return m_bEnableControlPort; }
  [[nodiscard]] bool isHashedPasswordAuthenticationEnabled() const { return m_bHashedPasswordAuthentication; }
  std::string getHashedControlPassword() { return m_sHashedControlPassword; }
  [[nodiscard]] uint16_t getControlPort() const { return m_u16ControlPort; }
  [[nodiscard]] bool isHiddenServiceEnabled() const { return m_bEnableHiddenService; }
  std::string getHiddenServiceDirectory() { return m_sHiddenServiceDirectory; }
  [[nodiscard]] uint16_t getHiddenServicePort() const { return m_u16HiddenServicePort; }

  // converter
  std::vector<char *> toArguments();
  std::string getProxyAddress() const;
};

class ITor {
 public:
  ITor() = default;
  ~ITor() = default;

  virtual const char *getVersion() = 0;
  virtual void start(bool i_Wait) = 0;
  virtual std::string hashPassword(const std::string &i_sValue) = 0;
  virtual bool isBootstrapped() = 0;
  virtual void waitUntilBootstrapped() = 0;
};

class Tor : public ITor {
  pthread_t m_Thread {};
  TorConfiguration m_Configuration;

 public:
  Tor() = default;
  explicit Tor(const TorConfiguration &i_Configuration) : m_Configuration(i_Configuration) {}
  ~Tor();

  const char *getVersion() override;
  void start(bool i_Wait) override;
  std::string hashPassword(const std::string &i_sValue) override;
  bool isBootstrapped() override;
  void waitUntilBootstrapped() override;

 protected:
  static bool start(std::vector<char *> i_Arguments);

  bool _start();
  static bool _start(TorConfiguration &i_Configuration);
  static void *_start_thread(void *i_Tor);
};
}

#endif//ONIONPP_ONIONPP_H

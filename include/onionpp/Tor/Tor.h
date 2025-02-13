//
// Created by nbdy on 03.12.24.
//

#ifndef TOR_H
#define TOR_H

#include "ITor.h"
#include "onionpp/Configuration/IConfiguration.h"

#include <thread>
#include <memory>

namespace onionpp {
class Tor final : public ITor {
  std::thread m_Thread{};
  IConfigurationPtr m_Configuration;

 public:
  Tor();
  explicit Tor(IConfigurationPtr  i_Configuration);
  ~Tor() override;

  bool start(bool i_WaitUntilBootstrapped) override;
  void stop() override;
  void join() override;
  bool isRunning() override;
  bool isBootstrapped() override;
  void waitUntilBootstrapped() override;
  uint8_t getBootstrapPercentage() override;
  IConfigurationPtr getConfiguration() override;

 protected:
  static void _start(Tor *i_Tor);
};
}// namespace onionpp

#endif //TOR_H

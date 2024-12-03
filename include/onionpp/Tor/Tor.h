//
// Created by nbdy on 03.12.24.
//

#ifndef TOR_H
#define TOR_H

#include "ITor.h"
#include "onionpp/Configuration/IConfiguration.h"

#include <memory>

namespace onionpp {
class Tor final : public ITor {
  pthread_t m_Thread{};
  IConfigurationPtr m_Configuration;

 public:
  Tor();
  explicit Tor(const IConfigurationPtr& i_Configuration);
  ~Tor() override;

  bool start(bool i_Wait = true) override;
  void stop() override;
  bool isBootstrapped() override;
  void waitUntilBootstrapped() override;
  uint8_t getBootstrapPercentage() override;
  IConfigurationPtr getConfiguration() override;

 protected:
  static void* _start(void* i_Configuration);
};
}// namespace onionpp

#endif //TOR_H

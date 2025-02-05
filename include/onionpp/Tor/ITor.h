//
// Created by nbdy on 03.12.24.
//

#ifndef ITOR_H
#define ITOR_H

#include <cstdint>

#include "onionpp/Configuration/IConfiguration.h"

namespace onionpp {
class ITor {
public:
  ITor() = default;
  virtual ~ITor() = default;

  virtual bool start(bool i_Wait) = 0;
  virtual void stop() = 0;
  virtual void join() = 0;
  virtual bool isRunning() = 0;
  virtual bool isBootstrapped() = 0;
  virtual void waitUntilBootstrapped() = 0;
  virtual uint8_t getBootstrapPercentage() = 0;
  virtual IConfigurationPtr getConfiguration() = 0;
};
}

#endif //ITOR_H

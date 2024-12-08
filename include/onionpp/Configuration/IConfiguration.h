//
// Created by nbdy on 03.12.24.
//

#ifndef ITORCONFIGURATION_H
#define ITORCONFIGURATION_H

#include <memory>

#include "onionpp/Option/Option.h"

namespace onionpp {
class IConfiguration {
public:
  IConfiguration() = default;
  virtual ~IConfiguration() = default;

  [[nodiscard]] virtual ConfigOptionMap getOptions() const = 0;

  [[nodiscard]] virtual std::string getOption(const Option& i_Option) const = 0;
  virtual void setOption(const Option& i_Option, const std::string& i_Value) = 0;

  [[nodiscard]] virtual std::string getSocks5Address() const = 0;
};
using IConfigurationPtr = std::shared_ptr<IConfiguration>;
};

#endif //ITORCONFIGURATION_H

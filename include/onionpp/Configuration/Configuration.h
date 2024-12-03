//
// Created by nbdy on 03.12.24.
//

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <cstdint>

#include "onionpp/Option/Option.h"
#include "IConfiguration.h"

namespace onionpp {
class Configuration final : public IConfiguration {
  ConfigOptionMap m_OptionMap;

  protected:
  void parseEnvironment();

  public:
  Configuration();
  explicit Configuration(uint16_t i_Socks5Port);

  ConfigOptionMap getOptions() const override;

  std::string getOption(const Option& i_Option) const override;
  void setOption(const Option& i_Option, const std::string& i_Value) override;

  std::string getSocks5Address() const override;
};
}

#endif //CONFIGURATION_H

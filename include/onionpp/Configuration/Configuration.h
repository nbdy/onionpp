//
// Created by nbdy on 03.12.24.
//

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <cstdint>

#include "IConfiguration.h"

namespace onionpp {
class Configuration final : public IConfiguration {
  ConfigOptionMap m_OptionMap;

  protected:
  void setDefaults();
  void parseEnvironment();
  void parseArguments(int argc, char** argv);

  public:
  /*!
   * Parses environment variables
   */
  Configuration();
  /*!
   * Parses environment variables and then sets socks port
   */
  explicit Configuration(uint16_t i_Socks5Port);
  /*!
   * Moves ConfigOptionMap to member variable.
   * Does NOT parse environment.
   */
  explicit Configuration(ConfigOptionMap  i_ConfigOptionMap);

  /*!
   * First parses the environment, then argv
   */
  explicit Configuration(int argc, char** argv);

  ConfigOptionMap getOptions() const override;

  std::string getOption(const Option& i_Option) const override;
  void setOption(const Option& i_Option, const std::string& i_Value) override;

  std::string getSocks5Address() const override;
};
}

#endif //CONFIGURATION_H

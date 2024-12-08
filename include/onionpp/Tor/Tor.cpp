//
// Created by nbdy on 03.12.24.
//

#include <utility>
#include <vector>
#include <bits/ranges_util.h>

#include "Tor.h"

#include "onionpp/Option/OptionMapping.h"
#include "onionpp/Configuration/Configuration.h"

extern "C" {
  typedef struct tor_main_configuration_t tor_main_configuration_t;
  tor_main_configuration_t *tor_main_configuration_new(void);
  void tor_main_configuration_free(tor_main_configuration_t *cfg);
  int tor_main_configuration_set_command_line(tor_main_configuration_t *cfg,
                                            int argc, char *argv[]);
  const char *tor_api_get_provider_version(void);
  int tor_run_main(const tor_main_configuration_t *);

  int control_get_bootstrap_percent(void);
  void tor_shutdown_event_loop_and_exit(int exitcode);
}

onionpp::Tor::Tor() {
  m_Configuration = std::make_shared<Configuration>();
}

onionpp::Tor::Tor(IConfigurationPtr i_Configuration): m_Configuration(std::move(i_Configuration)) {}

onionpp::Tor::~Tor() {
  stop();
}

bool onionpp::Tor::start(const bool i_Wait) {
  pthread_create(&m_Thread, nullptr, &Tor::_start, this);
  if (i_Wait) { waitUntilBootstrapped(); }
  return true;
}

void onionpp::Tor::stop() {
  tor_shutdown_event_loop_and_exit(0);
  pthread_join(m_Thread, nullptr);
}

void onionpp::Tor::join() {
  pthread_join(m_Thread, nullptr);
}

bool onionpp::Tor::isBootstrapped() {
  return control_get_bootstrap_percent() == 100;
}

void onionpp::Tor::waitUntilBootstrapped() {
  while (isBootstrapped() == false) { sleep(1); }
}

uint8_t onionpp::Tor::getBootstrapPercentage() {
  return control_get_bootstrap_percent();
}

onionpp::IConfigurationPtr onionpp::Tor::getConfiguration() {
  return m_Configuration;
}

void *onionpp::Tor::_start(void *i_Tor) {
  const auto tor = static_cast<Tor*>(i_Tor);
  tor_main_configuration_t *tor_cfg = tor_main_configuration_new();
  std::vector<std::string> strArgs;
  strArgs.emplace_back("tor");

  for (const auto& [key, value] : tor->getConfiguration()->getOptions()) {
    const auto it = std::ranges::find_if(OptionMapping,
                           [key](const ConfigOptionMapping& mapping) { return mapping.ConfigOption == key; });
    if (it != std::end(OptionMapping)) {
      strArgs.emplace_back(it->ArgVar);
      strArgs.push_back(value);
    }
  }

  std::vector<char*> args;
  for (auto& arg : strArgs) {
    args.push_back(arg.data());
  }

  args.push_back(nullptr);

  const int result = tor_main_configuration_set_command_line(
      tor_cfg, static_cast<int>(args.size() - 1), args.data());
  if (result != 0) {
    throw std::runtime_error("Failed to apply Tor configuration");
  }

  tor_run_main(tor_cfg);
  tor_main_configuration_free(tor_cfg);

  return nullptr;
}

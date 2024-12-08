#include <chrono>
#include <csignal>
#include <thread>
#include <iostream>

#include "CLI/CLI.hpp"

#include <onionpp/Configuration/Configuration.h>
#include <onionpp/Option/OptionMapping.h>
#include <onionpp/Tor/Tor.h>
#include <onionpp/onionpp.h>

bool bRun = true;

void handler(int) {
  bRun = false;
}

int main(const int argc, char** argv) {
#ifdef _WIN32
  signal(SIGINT, handler);
#else
  struct sigaction signalHandler {};
  signalHandler.sa_handler = handler;
  sigemptyset(&signalHandler.sa_mask);
  signalHandler.sa_flags = 0;
  sigaction(SIGINT, &signalHandler, nullptr);
#endif

  CLI::App app {"Tor++"};
  argv = app.ensure_utf8(argv);

  onionpp::ConfigOptionMap configuration {};
  for (const auto& mapping : onionpp::OptionMapping) {
    if (mapping.DefaultValue.empty() == false) {
      configuration[mapping.ConfigOption] = mapping.DefaultValue;
      app.add_option(mapping.ArgVar.data(), configuration[mapping.ConfigOption], mapping.Description.data());
    }
  }

  CLI11_PARSE(app, argc, argv);

  std::cout << "Version: " << onionpp::getVersion() << std::endl;
  onionpp::Configuration cfg(configuration);

  std::cout << "Created tor configuration" << std::endl;

  onionpp::Tor tor(std::make_shared<onionpp::Configuration>(cfg));
  std::cout << "Tor is running now and can be controlled via the control port." << std::endl;
  tor.start(true);

  return 0;
}

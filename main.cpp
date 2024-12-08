#include <chrono>
#include <csignal>
#include <thread>
#include <iostream>

#include "CLI/CLI.hpp"

#include "onionpp/onionpp.h"


bool bRun = true;

void handler(int _) {
  bRun = false;
}

int main(int argc, char** argv) {
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

  bool enableControlPortAuthentication = true;
  std::string controlPortPassword = "changeme";

  app.add_option("-cpp,--control-port-password", controlPortPassword, "The password used for control port authentication");
  app.add_flag("-dcpa,--disable-control-port-authentication", enableControlPortAuthentication, "Disable control port authentication");

  CLI11_PARSE(app, argc, argv);

  std::cout << "Version: " << onionpp::getVersion() << std::endl;
  const auto hashedPassword = onionpp::hashPassword(controlPortPassword);

  const auto cfg = std::make_shared<onionpp::Configuration>();
  cfg->setOption(onionpp::Option::ControlPort, "1");
  cfg->setOption(onionpp::Option::ControlPortHashedPassword, enableControlPortAuthentication ? "1" : "0");
  if (enableControlPortAuthentication) {
    cfg->setOption(onionpp::Option::HashedControlPassword, hashedPassword);
  }

  std::cout << "Created tor configuration" << std::endl;

  onionpp::Tor tor(cfg);
  std::cout << "Tor is running now and can be controlled via the control port." << std::endl;
  tor.join();

  return 0;
}

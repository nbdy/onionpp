#include <chrono>
#include <csignal>
#include <thread>
#include <iostream>

#include "CLI/CLI.hpp"

#include "onionpp/onionpp.h"


bool bRun = true;

void handler(int) {
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

  bool disableCookieAuthentication = false;
  std::string controlPortPassword = "changeme";

  app.add_option("--cookie", controlPortPassword, "Password used for the control port");
  app.add_flag("--cookie-authentication", disableCookieAuthentication, "Disable control port authentication");

  CLI11_PARSE(app, argc, argv);

  std::cout << "Version: " << onionpp::getVersion() << std::endl;
  const auto hashedPassword = onionpp::hashPassword(controlPortPassword);

  const auto cfg = std::make_shared<onionpp::Configuration>();
  cfg->setOption(onionpp::Option::ControlPort, "1");
  cfg->setOption(onionpp::Option::CookieAuthentication, disableCookieAuthentication ? "0" : "1");
  if (disableCookieAuthentication == false) {
    cfg->setOption(onionpp::Option::HashedControlPassword, hashedPassword);
  }

  std::cout << "Created tor configuration" << std::endl;

  onionpp::Tor tor(cfg);
  std::cout << "Tor is running now and can be controlled via the control port." << std::endl;
  tor.start(true);

  return 0;
}

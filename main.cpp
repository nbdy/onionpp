#include <csignal>
#include <iostream>
#include <unistd.h>

#include "onionpp/onionpp.h"

bool bRun = true;

void handler(int _) {
  bRun = false;
}

int main() {
#ifdef _WIN32
  signal(SIGINT, handler);
#else
  struct sigaction signalHandler {};
  signalHandler.sa_handler = handler;
  sigemptyset(&signalHandler.sa_mask);
  signalHandler.sa_flags = 0;
  sigaction(SIGINT, &signalHandler, nullptr);
#endif

  std::cout << "Version: " << onionpp::getVersion() << std::endl;
  const auto hashedPassword = onionpp::hashPassword("my_password");
  std::cout << hashedPassword << std::endl;

  auto cfg = std::make_shared<onionpp::Configuration>();
  cfg->setOption(onionpp::Option::ControlPort, "1");
  cfg->setOption(onionpp::Option::ControlPortHashedPassword, "1");
  cfg->setOption(onionpp::Option::HashedControlPassword, hashedPassword);;

  std::cout << "Created tor configuration" << std::endl;

  onionpp::Tor tor(cfg);
  std::cout << "Tor is running now and can be controlled via the control port." << std::endl;

  while(bRun) {
    usleep(500000);
  }

  return 0;
}

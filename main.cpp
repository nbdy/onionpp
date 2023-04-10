#include <csignal>
#include <iostream>
#include <unistd.h>

#include "onionpp.h"

bool bRun = true;

void handler(int i_iSignal) {
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

  auto tor = onionpp::Tor();

  std::cout << "Version: " << tor.getVersion() << std::endl;
  auto hashedPassword = tor.hashPassword("my_password");
  std::cout << hashedPassword << std::endl;

  auto cfg = onionpp::TorConfiguration();
  cfg.setControlPortEnabled(true);
  cfg.setHashedPasswordAuthenticationEnabled(true);
  cfg.setHashedControlPassword(hashedPassword);
  std::cout << "Created tor configuration" << std::endl;

  tor.start(cfg);
  std::cout << "Tor is running now and can be controlled via the control port." << std::endl;

  while(bRun) {
    usleep(500000);
  }

  return 0;
}

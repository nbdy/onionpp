#include <csignal>
#include <unistd.h>

#include "onionpp.h"

bool bRun = true;

void handler(int i_iSignal) {
  bRun = false;
}

int main() {
  struct sigaction signalHandler {};
  signalHandler.sa_handler = handler;
  sigemptyset(&signalHandler.sa_mask);
  signalHandler.sa_flags = 0;
  sigaction(SIGINT, &signalHandler, nullptr);

  std::cout << "Version: " << onionpp::Tor::getVersion() << std::endl;
  auto hashedPassword = onionpp::Tor::hashPassword("my_password");
  std::cout << hashedPassword << std::endl;

  auto cfg = onionpp::TorConfiguration();
  cfg.setControlPortEnabled(true);
  cfg.setHashedPasswordAuthenticationEnabled(true);
  cfg.setHashedControlPassword(hashedPassword);
  std::cout << "Created tor configuration" << std::endl;

  auto tor = onionpp::Tor(cfg);
  std::cout << "Tor is running now and can be controlled via the control port." << std::endl;

  while(bRun) {
    usleep(500000);
  }

  return 0;
}

#include "onionpp.h"

int main() {
  std::cout << "Version:" << onionpp::Tor::getVersion() << std::endl;
  auto hashedPassword = onionpp::Tor::hashPassword("my_password");
  std::cout << hashedPassword << std::endl;

  auto cfg = onionpp::TorConfiguration();
  cfg.setControlPortEnabled(true);
  cfg.setHashedPasswordAuthenticationEnabled(true);
  cfg.setHashedControlPassword(hashedPassword);

  auto tor = onionpp::Tor(cfg);
  std::cout << "Tor is running now and can be controlled via the control port." << std::endl;

  return 0;
}

#include <csignal>
#include <thread>
#include <iostream>

#include <onionpp/Configuration/Configuration.h>
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

  std::cout << "Version: " << onionpp::getVersion() << std::endl;
  onionpp::Configuration cfg(argc, argv);

  std::cout << "Created tor configuration" << std::endl;

  onionpp::Tor tor(std::make_shared<onionpp::Configuration>(cfg));
  std::cout << "Tor is running now and can be controlled via the control port." << std::endl;
  tor.start(true);
  tor.join();

  return 0;
}

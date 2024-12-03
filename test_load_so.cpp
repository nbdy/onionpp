//
// Created by nbdy on 10.04.23.
//

#include <filesystem>
#include <dlfcn.h>
#include <iostream>

#include "include/onionpp.h"

typedef onionpp::ITor* (*TorFactory)();

int main() {
  std::cout << "Current path: " << std::filesystem::current_path() << std::endl;
  const auto path = std::filesystem::current_path() / "libonionpp.so";
  std::cout << "Loading library: " << path.c_str() << std::endl;
  void *handle = dlopen(path.c_str(), RTLD_LAZY);
  if (handle == nullptr) {
    std::cout << "Could not load library: " << dlerror() << std::endl;
    return 0;
  }
  std::cout << "Loading create_tor symbol" << std::endl;
  const auto factory = reinterpret_cast<TorFactory>(dlsym(handle, "create_tor"));
  std::cout << "Creating tor instance" << std::endl;
  onionpp::ITor *tor = factory();
  std::cout << "Starting tor instance" << std::endl;
  tor->start(true);

  return 0;
}
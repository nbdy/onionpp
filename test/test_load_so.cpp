//
// Created by nbdy on 10.04.23.
//

#include <filesystem>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "../include/onionpp/Tor/ITor.h"

typedef onionpp::ITor* (*TorFactory)();

int main() {
  std::cout << "Current path: " << std::filesystem::current_path() << std::endl;

#ifdef _WIN32
  const auto path = std::filesystem::current_path() / "libonionpp-shared.dll";
#else
  const auto path = std::filesystem::current_path() / "libonionpp-shared.so";
#endif
  const std::string pathStr = path.string();

  std::cout << "Loading library: " << pathStr << std::endl;
#ifdef _WIN32
  HMODULE handle = LoadLibrary(pathStr.c_str());
  if (handle == nullptr) {
    std::cerr << "Could not load library: " << GetLastError() << std::endl;
    return 1;
  }
#else
  void* handle = dlopen(path.c_str(), RTLD_LAZY);
  if (handle == nullptr) {
    std::cerr << "Could not load library: " << dlerror() << std::endl;
    return 1;
  }
#endif

  std::cout << "Loading create_tor symbol" << std::endl;
#ifdef _WIN32
  const auto proc = GetProcAddress(handle, "create_tor");
  if (proc == nullptr) {
    std::cerr << "Could not load symbol: " << GetLastError() << std::endl;
    FreeLibrary(handle);
    return 1;
  }
  auto factory = reinterpret_cast<TorFactory>(reinterpret_cast<void*>(proc));
#else
  auto factory = reinterpret_cast<TorFactory>(dlsym(handle, "create_tor"));
  if (factory == nullptr) {
    std::cerr << "Could not load symbol: " << dlerror() << std::endl;
#ifdef _WIN32
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
    return 1;
  }
#endif

  std::cout << "Creating tor instance" << std::endl;
  onionpp::ITor *tor = factory();

  std::cout << "Starting tor instance" << std::endl;
  tor->start(true);

  std::cout << "Done. Cleaning up" << std::endl;
#ifdef _WIN32
  FreeLibrary(handle);
#else
  dlclose(handle);
#endif

  return 0;
}
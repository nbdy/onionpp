//
// Created by nbdy on 10.04.23.
//

#include "onionpp/onionpp.h"

#include "onionpp/Tor/ITor.h"
#include "onionpp/Tor/Tor.h"

extern "C" {
#ifdef _WIN32
  typedef int SOCKET;
#endif

  void crypto_rand(char*, int32_t);
  void base16_encode(char* dest, size_t destlen, const char* src, size_t srclen);
  void secret_to_key_rfc2440(
      char* key_out, size_t key_out_len, const char* secret,
      size_t secret_len, const char* s2k_specifier);
  const char* tor_api_get_provider_version();
}

std::string onionpp::hashPassword(const std::string& i_sValue) {
  char output[256] = {};
  char key[29];
  crypto_rand(key, 8);
  key[8] = static_cast<uint8_t>(96);
  secret_to_key_rfc2440(key + 9, 20, &i_sValue[0], i_sValue.size(), key);
  base16_encode(output, sizeof(output), key, sizeof(key));
  return "16:" + std::string(output);
}

const char* onionpp::getVersion() {
  return ONIONPP_VERSION;
}

const char *onionpp::getTorVersion() {
  return tor_api_get_provider_version();
}

extern "C" onionpp::ITor* create_tor() {
  return new onionpp::Tor();
}

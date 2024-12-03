#ifndef ONIONPP_ONIONPP_H
#define ONIONPP_ONIONPP_H

#include <string>

#include "Option/Option.h"
#include "Configuration/Configuration.h"
#include "Tor/Tor.h"

namespace onionpp {
static const char* getVersion();
static const char* getTorVersion();

static std::string hashPassword(const std::string& i_sValue);
}

#endif//ONIONPP_ONIONPP_H

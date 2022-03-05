//
// Created by nbdy on 26.02.22.
//

#include "onionpp.h"

#include <pybind11/pybind11.h>

#include <utility>

PYBIND11_MODULE(onionpp, m) {
  pybind11::class_<onionpp::Tor> tor(m, "Tor");
  tor.def(pybind11::init([] {
    return new onionpp::Tor;
  }));
  tor.def(pybind11::init([](const onionpp::TorConfiguration& i_Configuration) {
    return new onionpp::Tor(i_Configuration);
  }));

  tor.def("start", [](std::vector<char*> i_Arguments) {
    return onionpp::Tor::start(std::move(i_Arguments));
  });
  tor.def("start", [](const onionpp::TorConfiguration& i_Configuration) {
    return onionpp::Tor::start(i_Configuration);
  });

  tor.def("get_version", &onionpp::Tor::getVersion, pybind11::return_value_policy::reference);
  tor.def("hash_password", &onionpp::Tor::hashPassword, pybind11::return_value_policy::reference);

  pybind11::class_<onionpp::TorConfiguration> torConfiguration(m, "TorConfiguration");
  torConfiguration.def(pybind11::init([] {
    return new onionpp::TorConfiguration;
  }));
  torConfiguration.def(pybind11::init([](uint32_t i_u32SocksPort) {
    return new onionpp::TorConfiguration(i_u32SocksPort);
  }));
  torConfiguration.def("set_socks_port", &onionpp::TorConfiguration::setSocksPort);
  torConfiguration.def("get_socks_port", &onionpp::TorConfiguration::getSocksPort);
  torConfiguration.def_property("socks_port", &onionpp::TorConfiguration::getSocksPort, &onionpp::TorConfiguration::setSocksPort);

  torConfiguration.def("set_control_port_enabled", &onionpp::TorConfiguration::setControlPortEnabled);
  torConfiguration.def("is_control_port_enabled", &onionpp::TorConfiguration::isControlPortEnabled);
  torConfiguration.def_property("control_port_enabled", &onionpp::TorConfiguration::isControlPortEnabled, &onionpp::TorConfiguration::setControlPortEnabled);

  torConfiguration.def("set_password_authentication_enabled", &onionpp::TorConfiguration::setHashedPasswordAuthenticationEnabled);
  torConfiguration.def("is_password_authentication_enabled", &onionpp::TorConfiguration::isHashedPasswordAuthenticationEnabled);
  torConfiguration.def_property("control_password_authentication_enabled", &onionpp::TorConfiguration::isHashedPasswordAuthenticationEnabled, &onionpp::TorConfiguration::setHashedPasswordAuthenticationEnabled);

  torConfiguration.def("set_hashed_control_password", &onionpp::TorConfiguration::setHashedControlPassword);
  torConfiguration.def("get_hashed_control_password", &onionpp::TorConfiguration::getHashedControlPassword);
  torConfiguration.def_property("hashed_control_password", &onionpp::TorConfiguration::getHashedControlPassword, &onionpp::TorConfiguration::setHashedControlPassword);

  torConfiguration.def("set_control_port", &onionpp::TorConfiguration::setControlPort);
  torConfiguration.def("get_control_port", &onionpp::TorConfiguration::getControlPort);
  torConfiguration.def_property("control_port", &onionpp::TorConfiguration::getControlPort, &onionpp::TorConfiguration::setControlPort);

  torConfiguration.def("set_hidden_service_enabled", &onionpp::TorConfiguration::setHiddenServiceEnabled);
  torConfiguration.def("is_hidden_service_enabled", &onionpp::TorConfiguration::isHiddenServiceEnabled);
  torConfiguration.def_property("hidden_service_enabled", &onionpp::TorConfiguration::isHiddenServiceEnabled, &onionpp::TorConfiguration::setHiddenServiceEnabled);

  torConfiguration.def("set_hidden_service_directory", &onionpp::TorConfiguration::setHiddenServiceDirectory);
  torConfiguration.def("get_hidden_service_directory", &onionpp::TorConfiguration::getHiddenServiceDirectory);
  torConfiguration.def_property("hidden_service_directory", &onionpp::TorConfiguration::getHiddenServiceDirectory, &onionpp::TorConfiguration::setHiddenServiceDirectory);

  torConfiguration.def("set_hidden_service_port", &onionpp::TorConfiguration::setHiddenServicePort);
  torConfiguration.def("get_hidden_service_port", &onionpp::TorConfiguration::getHiddenServicePort);
  torConfiguration.def_property("hidden_service_port", &onionpp::TorConfiguration::getHiddenServicePort, &onionpp::TorConfiguration::setHiddenServicePort);
}

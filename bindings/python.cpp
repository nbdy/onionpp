//
// Created by nbdy on 26.02.22.
//

#include "onionpp/onionpp.h"
#include "onionpp/Option/Option.h"
#include "onionpp/Configuration/Configuration.h"
#include "onionpp/Tor/Tor.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

PYBIND11_MODULE(onionpy, module) {
  // ----------------------------
  pybind11::enum_<onionpp::Option> option(module, "Option");

  // ----------------------------
  pybind11::class_<onionpp::IConfiguration> torConfigurationInterface(module, "IConfiguration");
  pybind11::class_<onionpp::Configuration> torConfiguration(module, "Configuration");

  torConfiguration.def(pybind11::init([] {
    return new onionpp::Configuration;
  }));
  torConfiguration.def(pybind11::init([](const uint16_t i_SocksPort) {
    return new onionpp::Configuration(i_SocksPort);
  }));

  torConfiguration.def("get_option", &onionpp::Configuration::getOption, pybind11::arg("option"));
  torConfiguration.def("set_option", &onionpp::Configuration::setOption, pybind11::arg("option"), pybind11::arg("value"));

  torConfiguration.def("get_options", &onionpp::Configuration::getOptions);
  torConfiguration.def("get_socks5_address", &onionpp::Configuration::getSocks5Address);
  // ----------------------------
  pybind11::class_<onionpp::ITor> torInterface(module, "ITor");
  pybind11::class_<onionpp::Tor> tor(module, "Tor");

  tor.def(pybind11::init([] {
    return new onionpp::Tor;
  }));
  tor.def(pybind11::init([](const onionpp::IConfigurationPtr& i_Configuration) {
    return new onionpp::Tor(i_Configuration);
  }));

  tor.def("get_version", &onionpp::getVersion, pybind11::return_value_policy::reference);
  tor.def_static("hash_password", &onionpp::hashPassword);

  tor.def("start", pybind11::overload_cast<bool>(&onionpp::Tor::start), pybind11::arg("wait") = true);

  tor.def("is_bootstrapped", &onionpp::Tor::isBootstrapped);
  tor.def("wait_until_bootstrapped", &onionpp::Tor::waitUntilBootstrapped);
  tor.def("get_bootstrap_percentage", &onionpp::Tor::getBootstrapPercentage);
  tor.def("get_configuration", &onionpp::Tor::getConfiguration);
  // ----------------------------
}

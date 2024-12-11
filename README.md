# onionpp

| Status                                                                              |
|-------------------------------------------------------------------------------------|
| ![Linux](https://github.com/nbdy/onionpp/actions/workflows/linux.yml/badge.svg)     |
| ![Windows](https://github.com/nbdy/onionpp/actions/workflows/windows.yml/badge.svg) |

embed tor in your application

## features

- [X] start tor
  - [X] with inline configuration
  - [X] with environment variables

## usage

```c++
#include "onionpp.h"

int main() {
  onionpp::Tor tor;
  tor.start(true); // true to wait until tor has bootstrapped
  // else you can pass false and check via
  // tor.isBootstrapped or tor.getBootstrapPercentage

  // use the tor socks port
  return 0;
}
```

## bindings

- [X] [python](https://github.com/nbdy/onionpy)

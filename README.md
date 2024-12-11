# onionpp
[![Linux build](https://github.com/nbdy/onionpp/actions/workflows/linux.yml/badge.svg)](https://github.com/nbdy/onionpp/actions/workflows/linux.yml)
[![Windows 32 bit build](https://github.com/nbdy/onionpp/actions/workflows/windows-i686.yml/badge.svg)](https://github.com/nbdy/onionpp/actions/workflows/windows-i686.yml)

embed tor in your application

## requirements

### linux

- libevent-dev
- liblzma-dev
- libssl-dev
- zlib1g-dev

### windows

- all the linux dependencies
- mingw

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

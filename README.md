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
- [X] python bindings


- Size of onionpp_test build for
  - windows/i686: 10MB
  - linux/amd64: 8MB

## usage

```c++
#include "onionpp.h"

int main() {
  onionpp::Tor tor;

  // use the tor socks port
  return 0;
}
```

## screenshots

### windows

![onionpp_test](https://i.ibb.co/XXDgWyt/grafik.png)

### linux

![onionpp_test](https://i.ibb.co/t2LJxkB/grafik.png)

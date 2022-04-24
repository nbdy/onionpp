# onionpp

embed tor in your application

## requirements

### linux

- libevent-dev
- liblzma-dev
- libssl-dev
- libzstd-dev
- zlib1g-dev

## features

- [X] start tor
  - [X] with inline configuration
- [X] python bindings

## usage

```c++
#include "onionpp.h"

int main() {
  onionpp::Tor tor;

  // use the tor socks port
  return 0;
}
```

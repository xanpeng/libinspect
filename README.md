libinspect
==========

# What's this & how to use

A library which you can use to expose your program's internal information. A built-in HTTP server is used.

Of course you have to define what to expose, then register it as libinspect callback functions, and start the inspector.

A simple example (see more under examples/):

```cpp
#include <iostream>
#include <string>
#include "../libinspect/inspector.h"

static std::string DoList(const libinspect::Inspector::ArgList&) {
  return "dumb DoList";
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: http_server <port>\n";
    return 1; 
  }

  libinspect::Inspector inspector(argv[1]);
  inspector.AddContent("ls", "list status", DoList);

  inspector.Start();

  return 0;
}
```

# Inside libinspect

Inside libinspect, it has

- a simple HTTP server implemented by boost asio library (Actually code is basically stolen from boost asio HTTP server example).
- a callback register and call mechanism.

# TODO

- Unit test
- Support accept of parameters from request
- More examples

Please feel free to contact me: xanpeng@gmail.com


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


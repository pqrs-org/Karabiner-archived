#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "server.hpp"
#include "util.h"
#include "Mutex.hpp"

namespace {
  KeyRemap4MacBook_server::Server server;
}

int
server_initialize(const char* basedirectory)
{
  return server.initialize(basedirectory);
}

int
server_process(void)
{
  int error = 0;
  if (! server.process()) {
    std::cerr << "[ERROR] KeyRemap4MacBook_server: server.doLoop was finished for some reason." << std::endl;
    error = 1;
  }
  return error;
}


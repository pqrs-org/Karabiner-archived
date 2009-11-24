#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "server.hpp"
#include "util.h"

#include <CoreFoundation/CoreFoundation.h>
#include <SystemConfiguration/SystemConfiguration.h>

namespace {
  KeyRemap4MacBook_server::Server server;
}

int
server_initialize(const char* basedirectory)
{
  return server.initialize(basedirectory);
}

void
server_run(void)
{
  server.doLoop();
}

// ----------------------------------------------------------------------
namespace {
  KeyRemap4MacBook_server::Mutex mutex_sysctl;
}

void
sysctl_reset(void)
{
  KeyRemap4MacBook_server::Mutex::ScopedLock lk(mutex_sysctl);

  system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_reset terminate");
}

void
sysctl_load(void)
{
  KeyRemap4MacBook_server::Mutex::ScopedLock lk(mutex_sysctl);

  // --------------------------------------------------
  // check already initialized
  const char *name = "keyremap4macbook.initialized";

  int value;
  size_t len = sizeof(value);
  int error = sysctlbyname(name, &value, &len, NULL, 0);
  if (error) return;
  if (value != 0) return;

  // --------------------------------------------------
  int exitstatus;
  exitstatus = system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_reset");
  if (exitstatus != 0) return;

  exitstatus = system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set initialized 1");
  if (exitstatus != 0) return;

  exitstatus = system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_ctl load");
  if (exitstatus != 0) return;

  const std::string socket_path = server.getSocketPath();
  if (! socket_path.empty()) {
    std::string command = std::string("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set socket_path ") + socket_path;
    exitstatus = system(command.c_str());
    if (exitstatus != 0) return;
  }
}

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

// ----------------------------------------------------------------------
namespace {
  Mutex mutex_sysctl;

  bool
  wait_until_kext_loaded(void)
  {
    enum { TRY_COUNT = 60 };

    for (int i = 0; i < TRY_COUNT; ++i) {
      const char* name = "keyremap4macbook.initialized";
      int value;
      size_t len = sizeof(value);
      int error = sysctlbyname(name, &value, &len, NULL, 0);
      if (! error) return true;

      sleep(1);
    }
    return false;
  }
}

void
set_sysctl_do_reload_xml(void)
{
  if (! wait_until_kext_loaded()) {
    std::cerr << "[ERROR] failed to wait_until_kext_loaded" << std::endl;
  }

  // ----------------------------------------------------------------------
  Mutex::ScopedLock lk(mutex_sysctl);

  // --------------------------------------------------
  // check already initialized
  const char* name = "keyremap4macbook.initialized";

  int value;
  size_t len = sizeof(value);
  int error = sysctlbyname(name, &value, &len, NULL, 0);
  if (error) return;
  if (value != 0) return;

  // --------------------------------------------------
  // We need to set socket_path first,
  // because "do_reload_xml" use socket.
  const std::string socket_path = server.getSocketPath();
  if (! socket_path.empty()) {
    std::string command = std::string("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set socket_path ") + socket_path;
    error = system(command.c_str());
    if (error != 0) {
      std::cerr << "[ERROR] failed to sysctl_set socket_path" << std::endl;
      return;
    }
  }

  error = system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set do_reload_xml 1");
  if (error != 0) {
    std::cerr << "[ERROR] failed to sysctl_set do_reload_xml 1" << std::endl;
  }
}

void
set_sysctl_do_reset(void)
{
  if (! wait_until_kext_loaded()) {
    std::cerr << "[ERROR] failed to wait_until_kext_loaded" << std::endl;
  }

  // ----------------------------------------------------------------------
  Mutex::ScopedLock lk(mutex_sysctl);

  int error = system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set do_reset 1");
  if (error != 0) {
    std::cerr << "[ERROR] failed to sysctl_set do_reset 1" << std::endl;
  }
}

void
set_sysctl_do_reload_only_config(void)
{
  if (! wait_until_kext_loaded()) {
    std::cerr << "[ERROR] failed to wait_until_kext_loaded" << std::endl;
  }

  // ----------------------------------------------------------------------
  Mutex::ScopedLock lk(mutex_sysctl);

  int error = system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set do_reload_only_config 1");
  if (error != 0) {
    std::cerr << "[ERROR] failed to sysctl_set do_reload_only_config 1" << std::endl;
  }
}

#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>

namespace {
  void sigfunc(int param) {
    system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_reset terminate");
    exit(0);
  }

  bool doFirstReset(void) {
    // We wait till a user session is ready.
    const int MAX_TRY_COUNT = 5;

    for (int trycount = 0; trycount < MAX_TRY_COUNT; ++trycount) {
      int exitstatus = system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_reset");
      if (exitstatus == 0) return true;
    }
    return false;
  }
}

int
main()
{
  if (! doFirstReset()) {
    std::cerr << "[ERROR] KeyRemap4MacBook_sysctl_confd: can't reset configuration." << std::endl;
    exit(1);
  }

  signal(SIGHUP, sigfunc);
  signal(SIGTERM, sigfunc);

  for (;;) {
    const char *name = "keyremap4macbook.initialized";

    int value;
    size_t len = sizeof(value);
    int error = sysctlbyname(name, &value, &len, NULL, 0);
    if (error) {
      goto nextLoop;
    }
    if (value != 0) {
      goto nextLoop;
    }

    system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_reset");
    system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set initialized 1");
    system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_ctl load");

  nextLoop:
    sleep(3);
  }

  return 0;
}

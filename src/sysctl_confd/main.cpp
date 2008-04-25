#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

namespace {
  int exitLoop = false;

  void sigfunc(int param) {
    exitLoop = true;
  }
}

int
main()
{
  signal(SIGHUP, sigfunc);
  signal(SIGTERM, sigfunc);

  while (! exitLoop) {
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

    system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set initialized 1");
    system("/Library/org.pqrs/KeyRemap4MacBook/scripts/sysctl_reset.sh");
    system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_save --load");

  nextLoop:
    sleep(3);
  }

  system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set initialized 0");
  system("/Library/org.pqrs/KeyRemap4MacBook/scripts/sysctl_reset.sh");
  return 0;
}

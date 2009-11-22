#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

namespace {
  void sigfunc(int param) {
    system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_reset terminate");
    exit(0);
  }
}

int
main()
{
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

    int exitstatus;
    exitstatus = system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_reset");
    if (exitstatus != 0) goto nextLoop;

    exitstatus = system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_set initialized 1");
    if (exitstatus != 0) goto nextLoop;

    exitstatus = system("/Library/org.pqrs/KeyRemap4MacBook/bin/KeyRemap4MacBook_sysctl_ctl load");
    if (exitstatus != 0) goto nextLoop;

  nextLoop:
    sleep(1);
  }

  return 0;
}

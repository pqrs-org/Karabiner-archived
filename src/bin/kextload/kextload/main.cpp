#include <iostream>
#include <unistd.h>

namespace {
void usage(void) {
  std::cout << "Usage: kextload {load|unload}" << std::endl;
  exit(1);
}
}

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    usage();
  }

  std::string command;
  std::string argument(argv[1]);

  if (argument == "load") {
    command = "/sbin/kextload '/Applications/Karabiner.app/Contents/Library/Karabiner.signed.kext'";
  } else if (argument == "unload") {
    command = "/sbin/kextunload -b org.pqrs.driver.Karabiner";
  } else {
    usage();
  }

  // We need to call setuid in order to Execute command as root.
  // (rwsr-xr-x permission changes only euid (geteuid). So, we need to change uid by hand.)
  if (setuid(0) != 0) {
    std::cerr << "Failed to setuid(0)." << std::endl;
    exit(1);
  }
  return system(command.c_str());
}

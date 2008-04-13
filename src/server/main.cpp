#include <sys/types.h>
#include <sys/stat.h>
#include "server.hpp"

int
main()
{
  umask(0077);

  KeyRemap4MacBook_server::Server server;
  server.doLoop();

  return 0;
}

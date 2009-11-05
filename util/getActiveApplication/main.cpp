#include <iostream>
#include <stdlib.h>
#include "server_objc_part.h"

int
main()
{
  for (;;) {
    char applicationName[128];

    autoreleasepool_begin();
    getActiveApplicationName(applicationName, sizeof(applicationName));
    autoreleasepool_end();

    std::cout << applicationName << std::endl;
    sleep(1);
  }
}

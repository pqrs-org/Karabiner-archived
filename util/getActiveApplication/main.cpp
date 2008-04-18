#include <iostream>
#include <stdlib.h>
#include "getActiveApplicationName.h"

int
main()
{
  for (;;) {
    char applicationName[128];
    getActiveApplicationName(applicationName, sizeof(applicationName));
    std::cout << applicationName << std::endl;
    sleep(1);
  }
}

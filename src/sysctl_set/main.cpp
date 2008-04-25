#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
  if (argc != 3) {
    fprintf(stderr, "Usage: %s key value\n", argv[0]);
  }

  char name[512];
  snprintf(name, sizeof(name), "keyremap4macbook.%s", argv[1]);

  int value = atoi(argv[2]);

  size_t oldlen = 0;
  size_t newlen = sizeof(value);
  int error = sysctlbyname(name, NULL, &oldlen, &value, newlen);

  if (error) {
    perror("sysctl");
  }

  return 0;
}

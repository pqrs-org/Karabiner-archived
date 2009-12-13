#include <stdio.h>
#include "bundleprefix.h"
#include "SysctlWrapper.h"

int
main()
{
  NSString* version = [BUNDLEPREFIX(SysctlWrapper) getString:@"kern.ostype"];
  printf("%s\n", [version UTF8String]);
  return 0;
}

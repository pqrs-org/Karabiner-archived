#include <stdio.h>
#include "bundleprefix.h"
#include "SysctlWrapper.h"

int
main()
{
  NSString *version = [BUNDLEPREFIX_SysctlWrapper getString:@"keyremap4macbook.version"];
  NSLog(@"version = %s\n", [version UTF8String]);
  return 0;
}

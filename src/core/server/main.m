//
//  main.m
//  KeyRemap4MacBook_server
//
//  Created by Takayama Fumihiko on 09/11/01.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "util.h"
#include <signal.h>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    NSLog(@"Usage: KeyRemap4MacBook_server basedirectory");
    return 1;
  }

  signal(SIGPIPE, SIG_IGN);

  int success = server_initialize(argv[1]);
  if (! success) {
    NSLog(@"KeyRemap4MacBook_server: failed to server_initialize");
    return 1;
  }

  return NSApplicationMain(argc, (const char**) argv);
}

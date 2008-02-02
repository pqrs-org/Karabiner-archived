// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "AdminAction.h"

@implementation AdminAction

- (id) init
{
  [super init];

  _authorizationRef = nil;

  AuthorizationFlags flags = kAuthorizationFlagDefaults;
  OSStatus status = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, flags, &_authorizationRef);
  if (status != errAuthorizationSuccess) return nil;

  return self;
}

- (BOOL) getPrivilege
{
  if (! _authorizationRef) return FALSE;

  AuthorizationItem item;

  char path[] = "/bin/sh";
  item.name = kAuthorizationRightExecute;
  item.value = path;
  item.valueLength = strlen(path);
  item.flags = 0;

  AuthorizationRights rights;
  rights.count = 1;
  rights.items = &item;

  AuthorizationFlags flags = kAuthorizationFlagInteractionAllowed | kAuthorizationFlagExtendRights;
  OSStatus status = AuthorizationCopyRights(_authorizationRef, &rights, kAuthorizationEmptyEnvironment, flags, NULL);
  if (status != errAuthorizationSuccess) return FALSE;

  return TRUE;
}

- (BOOL) execCommand:(char *)command
{
  if (! [self getPrivilege]) return FALSE;

  char *const args[] = {
    "-c",
    command,
    NULL,
  };
  OSStatus status = AuthorizationExecuteWithPrivileges(_authorizationRef,
                                                       "/bin/sh",
                                                       kAuthorizationFlagDefaults,
                                                       args,
                                                       NULL);
  if (status != errAuthorizationSuccess) return FALSE;

  // wait child process
  int stat_loc;
  pid_t pid = wait(&stat_loc);
  if (pid == -1) return FALSE;

  return TRUE;
}

- (BOOL) setSysctlInt:(NSString *)name value:(NSNumber *)value
{
  char command[512];
  snprintf(command, sizeof(command), "/usr/sbin/sysctl -w '%s=%d'",
           [name cStringUsingEncoding:NSUTF8StringEncoding],
           [value intValue]);
  return [self execCommand:command];
}

- (BOOL) saveSetting
{
  char command[] = "/Applications/KeyRemap4MacBook/scripts/save.sh";
  return [self execCommand:command];
}

@end

// -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*-

#import "AdminAction.h"

static AuthorizationRef authorizationRef = nil;

@implementation BUNDLEPREFIX (AdminAction)

+ (BOOL) setup
{
  if (authorizationRef == nil) {
    AuthorizationFlags flags = kAuthorizationFlagDefaults;
    OSStatus status = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, flags, &authorizationRef);
    if (status != errAuthorizationSuccess) return FALSE;
  }
  return TRUE;
}

+ (BOOL) getPrivilege
{
  if (! [BUNDLEPREFIX(AdminAction) setup]) return FALSE;

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
  OSStatus status = AuthorizationCopyRights(authorizationRef, &rights, kAuthorizationEmptyEnvironment, flags, NULL);
  if (status != errAuthorizationSuccess) return FALSE;

  return TRUE;
}

+ (BOOL) execCommand:(char*)command
{
  if (! [BUNDLEPREFIX(AdminAction) getPrivilege]) return FALSE;

  char* const args[] = {
    "-c",
    command,
    NULL,
  };
  OSStatus status = AuthorizationExecuteWithPrivileges(authorizationRef,
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

@end

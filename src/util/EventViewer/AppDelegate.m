#import "AppDelegate.h"
#import "KarabinerClient.h"
#import "KarabinerKeys.h"
#import "KeyResponder.h"
#import "MigrationUtilities.h"
#import "PreferencesKeys.h"
#import "Relauncher.h"
#import <Carbon/Carbon.h>

@implementation AppDelegate

@synthesize window;

- (void)setKeyResponder {
  [window makeFirstResponder:keyResponder_];
}

- (NSString*)joinNamesFromIds:(NSString*)type ids:(NSArray*)ids {
  NSMutableString* result = [NSMutableString new];

  for (NSNumber* n in ids) {
    NSString* name = [[client_ proxy] symbolMapName:type value:[n intValue]];
    if (name) {
      if ([result length] > 0) {
        [result appendString:@", "];
      }
      // When type == @"ApplicationType", name == @"ApplicationType::NAME".
      // We make a substring which does not contain @"ApplicationType::".
      [result appendString:[name substringFromIndex:[type length] + 2]];
    }
  }

  if ([result length] == 0) {
    return @"---";
  }

  return result;
}

- (void)addToAppQueue {
  @synchronized(self) {
    static NSNumber* lastMtime = nil;

    @try {
      NSDictionary* information = [[client_ proxy] focused_uielement_information];
      if (information[@"mtime"] && ![lastMtime isEqualToNumber:information[@"mtime"]]) {
        lastMtime = information[@"mtime"];

        [appQueue_ push:information];

        // ----------------------------------------
        // set labels
        [label_appnames_ setStringValue:[self joinNamesFromIds:@"ApplicationType"
                                                           ids:[[client_ proxy] workspace_app_ids]]];

        [label_windownames_ setStringValue:[self joinNamesFromIds:@"WindowName"
                                                              ids:[[client_ proxy] workspace_window_name_ids]]];
      }
    }
    @catch (NSException* exception) {
      NSLog(@"%@", exception);
    }
  }
}

- (void)updateOtherInformationStore {
  @synchronized(self) {
    static NSNumber* lastMtime = nil;

    @try {
      NSDictionary* information = [[client_ proxy] inputsource_information];
      if (information[@"mtime"] && ![lastMtime isEqualToNumber:information[@"mtime"]]) {
        lastMtime = information[@"mtime"];

        [otherinformationstore_ setLanguageCode:information[@"languageCode"]];
        [otherinformationstore_ setInputSourceID:information[@"inputSourceID"]];
        [otherinformationstore_ setInputModeID:information[@"inputModeID"]];

        [label_input_sources_ setStringValue:[self joinNamesFromIds:@"InputSource"
                                                                ids:[[client_ proxy] workspace_inputsource_ids]]];
      }
    }
    @catch (NSException* exception) {
      NSLog(@"%@", exception);
    }
  }
}

// ------------------------------------------------------------
- (void)distributedObserver_kKarabinerServerDidLaunchNotification:(NSNotification*)notification {
  dispatch_async(dispatch_get_main_queue(), ^{
    [NSTask launchedTaskWithLaunchPath:[[NSBundle mainBundle] executablePath] arguments:@[]];
    [NSApp terminate:self];
  });
}

// ------------------------------------------------------------
- (void)timerFireMethod:(NSTimer*)timer {
  dispatch_async(dispatch_get_main_queue(), ^{
    [self addToAppQueue];
    [self updateOtherInformationStore];
  });
}

// ------------------------------------------------------------
- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
  if ([MigrationUtilities migrate:@[ @"org.pqrs.KeyRemap4MacBook.EventViewer" ]
           oldApplicationSupports:@[]
                         oldPaths:@[ @"/Applications/KeyRemap4MacBook.app/Contents/Applications/EventViewer.app" ]]) {
    [Relauncher relaunch];
  }

  // ----------------------------------------
  [self setKeyResponder];
  [self updateOtherInformationStore];
  [self setWindowProperty:self];

  [NSTimer scheduledTimerWithTimeInterval:0.3
                                   target:self
                                 selector:@selector(timerFireMethod:)
                                 userInfo:nil
                                  repeats:YES];

  // We need to speficy NSNotificationSuspensionBehaviorDeliverImmediately for NSDistributedNotificationCenter
  // in order to get notifications when this app is not active.
  [[NSDistributedNotificationCenter defaultCenter] addObserver:self
                                                      selector:@selector(distributedObserver_kKarabinerServerDidLaunchNotification:)
                                                          name:kKarabinerServerDidLaunchNotification
                                                        object:nil
                                            suspensionBehavior:NSNotificationSuspensionBehaviorDeliverImmediately];

  [Relauncher resetRelaunchedCount];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication {
  return YES;
}

- (void)dealloc {
  [[NSDistributedNotificationCenter defaultCenter] removeObserver:self];
}

- (void)tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem {
  if ([[tabViewItem identifier] isEqualToString:@"Main"]) {
    [self setKeyResponder];
  } else if ([[tabViewItem identifier] isEqualToString:@"Devices"]) {
    [devices_ refresh:nil];
  }
}

- (IBAction)setWindowProperty:(id)sender {
  // ----------------------------------------
  if ([[NSUserDefaults standardUserDefaults] boolForKey:kForceStayTop]) {
    [window setLevel:NSFloatingWindowLevel];

    NSWindowCollectionBehavior behavior = [window collectionBehavior];
    behavior &= ~(NSWindowCollectionBehaviorTransient);
    behavior |= NSWindowCollectionBehaviorManaged;
    [window setCollectionBehavior:behavior];

  } else {
    [window setLevel:NSNormalWindowLevel];

    NSWindowCollectionBehavior behavior = [window collectionBehavior];
    behavior &= ~(NSWindowCollectionBehaviorTransient);
    behavior |= NSWindowCollectionBehaviorManaged;
    [window setCollectionBehavior:behavior];
  }

  // ----------------------------------------
  if ([[NSUserDefaults standardUserDefaults] boolForKey:kShowInAllSpaces]) {
    NSWindowCollectionBehavior behavior = [window collectionBehavior];
    behavior &= ~(NSWindowCollectionBehaviorMoveToActiveSpace);
    behavior |= NSWindowCollectionBehaviorCanJoinAllSpaces;
    [window setCollectionBehavior:behavior];

  } else {
    NSWindowCollectionBehavior behavior = [window collectionBehavior];
    behavior &= ~(NSWindowCollectionBehaviorCanJoinAllSpaces | NSWindowCollectionBehaviorMoveToActiveSpace);
    [window setCollectionBehavior:behavior];
  }
}

@end

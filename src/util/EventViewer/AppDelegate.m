@import Carbon;
#import "AppDelegate.h"
#import "AppQueue.h"
#import "Devices.h"
#import "KeyResponder.h"
#import "MigrationUtilities.h"
#import "OtherInformationStore.h"
#import "PreferencesKeys.h"
#import "Relauncher.h"
#import "ServerClient.h"
#import "SharedKeys.h"
#import "weakify.h"

@interface AppDelegate ()

@property(weak) IBOutlet NSWindow* window;
@property(weak) IBOutlet AppQueue* appQueue;
@property(weak) IBOutlet Devices* devices;
@property(weak) IBOutlet ServerClient* client;
@property(weak) IBOutlet OtherInformationStore* otherInformationStore;
@property(weak) IBOutlet KeyResponder* keyResponder;
@property(weak) IBOutlet NSTextField* label_appnames;
@property(weak) IBOutlet NSTextField* label_windownames;
@property(weak) IBOutlet NSTextField* label_input_sources;

@end

@implementation AppDelegate

- (void)setKeyResponder {
  [self.window makeFirstResponder:self.keyResponder];
}

- (NSString*)joinNamesFromIds:(NSString*)type ids:(NSArray*)ids {
  NSMutableString* result = [NSMutableString new];

  for (NSNumber* n in ids) {
    NSString* name = [[self.client proxy] symbolMapName:type value:[n intValue]];
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
      NSDictionary* information = [[self.client proxy] focused_uielement_information];
      if (information[@"mtime"] && ![lastMtime isEqualToNumber:information[@"mtime"]]) {
        lastMtime = information[@"mtime"];

        [self.appQueue push:information];

        // ----------------------------------------
        // set labels
        [self.label_appnames setStringValue:[self joinNamesFromIds:@"ApplicationType"
                                                               ids:[[self.client proxy] workspace_app_ids]]];

        [self.label_windownames setStringValue:[self joinNamesFromIds:@"WindowName"
                                                                  ids:[[self.client proxy] workspace_window_name_ids]]];
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
      NSDictionary* information = [[self.client proxy] inputsource_information];
      if (information[@"mtime"] && ![lastMtime isEqualToNumber:information[@"mtime"]]) {
        lastMtime = information[@"mtime"];

        [self.otherInformationStore setLanguageCode:information[@"languageCode"]];
        [self.otherInformationStore setInputSourceID:information[@"inputSourceID"]];
        [self.otherInformationStore setInputModeID:information[@"inputModeID"]];

        [self.label_input_sources setStringValue:[self joinNamesFromIds:@"InputSource"
                                                                    ids:[[self.client proxy] workspace_inputsource_ids]]];
      }
    }
    @catch (NSException* exception) {
      NSLog(@"%@", exception);
    }
  }
}

// ------------------------------------------------------------
- (void)distributedObserver_kKarabinerServerDidLaunchNotification:(NSNotification*)notification {
  @weakify(self);

  dispatch_async(dispatch_get_main_queue(), ^{
    @strongify(self);
    if (!self) return;

    [NSTask launchedTaskWithLaunchPath:[[NSBundle mainBundle] executablePath] arguments:@[]];
    [NSApp terminate:self];
  });
}

// ------------------------------------------------------------
- (void)timerFireMethod:(NSTimer*)timer {
  @weakify(self);

  dispatch_async(dispatch_get_main_queue(), ^{
    @strongify(self);
    if (!self) return;

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
    [self.devices refresh];
  }
}

- (IBAction)setWindowProperty:(id)sender {
  // ----------------------------------------
  if ([[NSUserDefaults standardUserDefaults] boolForKey:kForceStayTop]) {
    [self.window setLevel:NSFloatingWindowLevel];

    NSWindowCollectionBehavior behavior = [self.window collectionBehavior];
    behavior &= ~(NSWindowCollectionBehaviorTransient);
    behavior |= NSWindowCollectionBehaviorManaged;
    [self.window setCollectionBehavior:behavior];

  } else {
    [self.window setLevel:NSNormalWindowLevel];

    NSWindowCollectionBehavior behavior = [self.window collectionBehavior];
    behavior &= ~(NSWindowCollectionBehaviorTransient);
    behavior |= NSWindowCollectionBehaviorManaged;
    [self.window setCollectionBehavior:behavior];
  }

  // ----------------------------------------
  if ([[NSUserDefaults standardUserDefaults] boolForKey:kShowInAllSpaces]) {
    NSWindowCollectionBehavior behavior = [self.window collectionBehavior];
    behavior &= ~(NSWindowCollectionBehaviorMoveToActiveSpace);
    behavior |= NSWindowCollectionBehaviorCanJoinAllSpaces;
    [self.window setCollectionBehavior:behavior];

  } else {
    NSWindowCollectionBehavior behavior = [self.window collectionBehavior];
    behavior &= ~(NSWindowCollectionBehaviorCanJoinAllSpaces | NSWindowCollectionBehaviorMoveToActiveSpace);
    [self.window setCollectionBehavior:behavior];
  }
}

@end

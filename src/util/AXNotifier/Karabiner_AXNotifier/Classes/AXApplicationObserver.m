#import "AXApplicationObserver.h"
#import "AXUtilities.h"
#import "GlobalAXNotifierPreferencesModel.h"
#import "NotificationKeys.h"
#import "PreferencesModel.h"

@interface AXApplicationObserver ()

@property(readwrite) NSRunningApplication* runningApplication;
@property(copy) NSString* title;
@property(copy) NSString* role;
@property AXUIElementRef applicationElement;
@property AXUIElementRef focusedWindowElementForAXTitleChangedNotification;
@property AXObserverRef observer;

- (void)updateTitle;
- (void)updateRole:(AXUIElementRef)element;

@end

static void
observerCallback(AXObserverRef observer, AXUIElementRef element, CFStringRef notification, void* refcon) {
  AXApplicationObserver* self = (__bridge AXApplicationObserver*)(refcon);
  if (!self) return;

  @synchronized(self) {
    if (CFStringCompare(notification, kAXTitleChangedNotification, 0) == kCFCompareEqualTo) {
      [self updateTitle];
      [self postNotification];
    }
    if (CFStringCompare(notification, kAXFocusedUIElementChangedNotification, 0) == kCFCompareEqualTo) {
      [self updateTitle];
      [self updateRole:element];
      [self postNotification];
    }
    if (CFStringCompare(notification, kAXFocusedWindowChangedNotification, 0) == kCFCompareEqualTo) {
      // ----------------------------------------
      // refresh observer.
      [self observeTitleChangedNotification];

      [self updateTitle];
      [self postNotification];
    }
  }
}

@implementation AXApplicationObserver

- (instancetype)initWithRunningApplication:(NSRunningApplication*)runningApplication {
  self = [super init];

  if (self) {
    _runningApplication = runningApplication;
    _title = @"";
    _role = @"";
  }

  return self;
}

- (void)dealloc {
  if (_observer) {
    CFRunLoopRemoveSource(CFRunLoopGetCurrent(),
                          AXObserverGetRunLoopSource(_observer),
                          kCFRunLoopDefaultMode);
    CFRelease(_observer);
    _observer = NULL;
  }

  if (_applicationElement) {
    CFRelease(_applicationElement);
    _applicationElement = NULL;
  }
  if (_focusedWindowElementForAXTitleChangedNotification) {
    CFRelease(_focusedWindowElementForAXTitleChangedNotification);
    _focusedWindowElementForAXTitleChangedNotification = NULL;
  }
}

- (void)observe {
  bool observable = YES;
  if (!AXIsProcessTrusted()) {
    observable = NO;
  }

  // We do not need to observe EventViewer because all events in EventViewer will be ignored in Karabiner.app.
  if ([[self.runningApplication bundleIdentifier] isEqualToString:@"org.pqrs.Karabiner.EventViewer"]) {
    observable = NO;
  }

  AXNotifierPreferencesModel* axNotifierPreferencesModel = [GlobalAXNotifierPreferencesModel get];

  // Java apps will be crashed if observe. (We confirm crash in SQLDeveloper.)
  if (axNotifierPreferencesModel.disabledInJavaApps) {
    if ([[[self.runningApplication executableURL] absoluteString] hasSuffix:@"/java"] ||
        [[[self.runningApplication executableURL] absoluteString] hasSuffix:@"/JavaApplicationStub"] ||
        [[[self.runningApplication executableURL] absoluteString] hasSuffix:@"/JavaAppLauncher"] ||

        // Eclipse
        [[[self.runningApplication executableURL] absoluteString] hasSuffix:@"/eclipse"] ||

        // LibreOffice
        // https://github.com/tekezo/Karabiner/issues/243
        [[self.runningApplication bundleIdentifier] isEqualToString:@"org.libreoffice.script"] ||

        // Matlab
        // https://github.com/tekezo/Karabiner/issues/259
        [[self.runningApplication bundleIdentifier] isEqualToString:@"com.mathworks.matlab"] ||

        // IntelliJ IDEA (com.jetbrains.intellij*)
        // PhpStorm (com.jetbrains.PhpStorm)
        // RubyMine (com.jetbrains.rubymine)
        // https://groups.google.com/d/msg/osx-karabiner/Ma0Bt2I2D-k/WiajWwueUQkJ
        //
        // We treat JetBrains's products are made by Java.
        [[self.runningApplication bundleIdentifier] hasPrefix:@"com.jetbrains."] ||

        // Android Studio
        // https://github.com/tekezo/Karabiner/issues/255
        [[self.runningApplication bundleIdentifier] isEqualToString:@"com.google.android.studio"] ||

        // Screencast-O-Matic
        // https://github.com/tekezo/Karabiner/issues/337
        [[self.runningApplication bundleIdentifier] isEqualToString:@"com.screencastomatic.app"] ||

        // RazorSQL
        // https://groups.google.com/d/msg/osx-karabiner/Q--m95davC4/vIpKpkpUongJ
        [[self.runningApplication bundleIdentifier] isEqualToString:@"com.razorsql.RazorSQL"] ||

        // EditRocket
        // https://groups.google.com/d/msg/osx-karabiner/Q--m95davC4/vIpKpkpUongJ
        [[self.runningApplication bundleIdentifier] isEqualToString:@"com.editrocket.EditRocket"] ||

        // Spine
        [[[self.runningApplication executableURL] absoluteString] hasSuffix:@"/SpineTrial"] ||
        [[[self.runningApplication executableURL] absoluteString] hasSuffix:@"/Spine"] ||

        // Fiji
        // https://github.com/tekezo/Karabiner/issues/522
        [[self.runningApplication bundleIdentifier] isEqualToString:@"org.fiji"] ||

        // Putting false in order to allow tailing || in the last valid item.
        false) {
      observable = NO;
    }
  }

  // Qt apps will be crashed if observe.
  if (axNotifierPreferencesModel.disabledInQtApps) {
    if ([[[self.runningApplication bundleIdentifier] lowercaseString] hasPrefix:@"com.buhldata."] ||
        false) {
      observable = NO;
    }
  }

  // Preview.app will be slow when opening large pdf if Preview.app is observed.
  // eg. http://web.mit.edu/rsi/www/pdfs/beamer-tutorial.pdf
  if (axNotifierPreferencesModel.disabledInPreview) {
    if ([[self.runningApplication bundleIdentifier] isEqualToString:@"com.apple.Preview"]) {
      observable = NO;
    }
  }

  // Microsoft Excel.app will reset scrolling by scroll wheel if it is observed.
  if (axNotifierPreferencesModel.disabledInMicrosoftOffice) {
    if ([[self.runningApplication bundleIdentifier] isEqualToString:@"com.microsoft.Excel"] ||
        [[self.runningApplication bundleIdentifier] isEqualToString:@"com.microsoft.Powerpoint"] ||
        [[self.runningApplication bundleIdentifier] isEqualToString:@"com.microsoft.Word"] ||
        false) {
      observable = NO;
    }
  }

  // ----------------------------------------
  if (observable) {
    pid_t pid = [self.runningApplication processIdentifier];

    // ----------------------------------------
    // Create applicationElement

    self.applicationElement = AXUIElementCreateApplication(pid);
    if (!self.applicationElement) {
      @throw [NSException exceptionWithName:@"AXApplicationObserverException"
                                     reason:@"AXUIElementCreateApplication is failed."
                                   userInfo:@{ @"runningApplication" : self.runningApplication }];
    }

    // ----------------------------------------
    // Create observer

    {
      AXObserverRef o;
      AXError error = AXObserverCreate(pid, observerCallback, &o);
      if (error != kAXErrorSuccess) {
        @throw [NSException exceptionWithName:@"AXApplicationObserverException"
                                       reason:@"AXObserverCreate is failed."
                                     userInfo:@{ @"runningApplication" : self.runningApplication,
                                                 @"error" : @(error) }];
      }
      self.observer = o;
    }

    // ----------------------------------------
    // Observe notifications

    // AXObserverAddNotification might be failed when just application launched.
    if (![self observeAXNotification:self.applicationElement notification:kAXFocusedUIElementChangedNotification add:YES]) {
      @throw [NSException exceptionWithName:@"AXApplicationObserverException"
                                     reason:@"Failed to observe kAXFocusedUIElementChangedNotification."
                                   userInfo:@{ @"runningApplication" : self.runningApplication }];
    }
    if (![self observeAXNotification:self.applicationElement notification:kAXFocusedWindowChangedNotification add:YES]) {
      @throw [NSException exceptionWithName:@"AXApplicationObserverException"
                                     reason:@"Failed to observe kAXFocusedWindowChangedNotification."
                                   userInfo:@{ @"runningApplication" : self.runningApplication }];
    }

    [self updateTitle];
    [self updateRole:NULL];

    // ----------------------------------------
    CFRunLoopAddSource(CFRunLoopGetCurrent(),
                       AXObserverGetRunLoopSource(self.observer),
                       kCFRunLoopDefaultMode);
  }

  if ([GlobalAXNotifierPreferencesModel debuggingLogEnabled]) {
    if (!observable) {
      NSString* path = [[self.runningApplication executableURL] absoluteString];
      NSLog(@"Ignore app: %@", path);
    }
  }
}

- (BOOL)observeAXNotification:(AXUIElementRef)element notification:(CFStringRef)notification add:(BOOL)add {
  if (!element) return YES;
  if (!self.observer) return YES;

  if (add) {
    AXError error = AXObserverAddNotification(self.observer,
                                              element,
                                              notification,
                                              (__bridge void*)self);
    if (error != kAXErrorSuccess) {
      if (error == kAXErrorNotificationUnsupported ||
          error == kAXErrorNotificationAlreadyRegistered) {
        // We ignore this error.
        return YES;
      }
      if ([GlobalAXNotifierPreferencesModel debuggingLogEnabled]) {
        NSLog(@"AXObserverAddNotification is failed: error:%@ %@", [AXUtilities errorString:error], self.runningApplication);
      }
      return NO;
    }

  } else {
    AXError error = AXObserverRemoveNotification(self.observer,
                                                 element,
                                                 notification);
    if (error != kAXErrorSuccess) {
      // Note: Ignore kAXErrorInvalidUIElement because it is expected error when focused window is closed.
      if (error == kAXErrorInvalidUIElement ||
          error == kAXErrorNotificationNotRegistered) {
        // We ignore this error.
        return YES;
      }
      if ([GlobalAXNotifierPreferencesModel debuggingLogEnabled]) {
        NSLog(@"AXObserverRemoveNotification is failed: error:%@ %@", [AXUtilities errorString:error], self.runningApplication);
      }
      return NO;
    }
  }

  return YES;
}

- (void)unobserveTitleChangedNotification {
  if (self.focusedWindowElementForAXTitleChangedNotification) {
    [self observeAXNotification:self.focusedWindowElementForAXTitleChangedNotification
                   notification:kAXTitleChangedNotification
                            add:NO];

    CFRelease(self.focusedWindowElementForAXTitleChangedNotification);
    self.focusedWindowElementForAXTitleChangedNotification = NULL;
  }
}

- (void)observeTitleChangedNotification {
  if (!self.applicationElement) return;

  [self unobserveTitleChangedNotification];

  self.focusedWindowElementForAXTitleChangedNotification = [AXUtilities copyFocusedWindow:self.applicationElement];
  if (!self.focusedWindowElementForAXTitleChangedNotification) return;

  [self observeAXNotification:self.focusedWindowElementForAXTitleChangedNotification
                 notification:kAXTitleChangedNotification
                          add:YES];
}

- (void)updateTitle {
  self.title = @"";

  if (!self.applicationElement) return;

  // Do not cache focusedWindowElement.
  // We need to get new focusedWindowElement because
  // getting title will be failed with cached focusedWindowElement on Finder.app.

  AXUIElementRef focusedWindowElement = [AXUtilities copyFocusedWindow:self.applicationElement];
  if (focusedWindowElement) {
    NSString* title = [AXUtilities titleOfUIElement:focusedWindowElement];
    if (title) {
      self.title = title;
    }
    CFRelease(focusedWindowElement);
  }
}

- (void)updateRole:(AXUIElementRef)element {
  self.role = @"";

  if (element) {
    CFRetain(element);
  } else {
    element = [AXUtilities copyFocusedUIElement];
  }
  if (element) {
    NSString* role = [AXUtilities roleOfUIElement:element];
    if (role) {
      self.role = role;
    }
    CFRelease(element);
  }
}

- (void)postNotification {
  NSDictionary* userInfo = @{
    @"bundleIdentifier" : [self.runningApplication bundleIdentifier],
    @"title" : self.title,
    @"role" : self.role,
  };
  [[NSNotificationCenter defaultCenter] postNotificationName:kFocusedUIElementChanged object:self userInfo:userInfo];
}

@end

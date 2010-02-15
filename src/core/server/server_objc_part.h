/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#ifndef SERVER_OBJC_PART_H
#define SERVER_OBJC_PART_H

typedef enum {
  STATUSMESSAGETYPE_NONE,
  STATUSMESSAGETYPE_LOCK,
  STATUSMESSAGETYPE_EXTRA,
} StatusMessageType;

#ifndef __cplusplus

#import <Cocoa/Cocoa.h>

@interface ServerObjcPart : NSObject {
  NSWindow* statuswindow_;
  NSTextField* statuswindow_label_;

  NSString* statusmessage_lock_;
  NSString* statusmessage_extra_;
}

- (NSString*) getActiveApplicationName;
- (NSString*) getTISPropertyInputModeID;

- (void) registerStatusWindow:(NSWindow*)window label:(NSTextField*)label;
- (void) updateStatusMessageWindow;
- (void) setStatusMessage:(StatusMessageType)type message:(const char*)message;

@end

// ------------------------------------------------------------
void registerServerObjcPart(ServerObjcPart* object);
void registerStatusWindow(NSWindow* window, NSTextField* label);

#endif

// ======================================================================
#ifdef __cplusplus
extern "C" {
#endif

void selectInputSource_english(void);
void selectInputSource_french(void);
void selectInputSource_german(void);
void selectInputSource_japanese(void);
void selectInputSource_swedish(void);
void selectInputSource_canadian(void);

void set_statusmessage(StatusMessageType type, const char* message);

#ifdef __cplusplus
}
#endif

#endif

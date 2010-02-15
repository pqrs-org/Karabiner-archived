/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#ifndef SERVER_OBJC_PART_H
#define SERVER_OBJC_PART_H

#ifndef __cplusplus

#import <Cocoa/Cocoa.h>

@interface ServerObjcPart : NSObject {}

- (NSString*) getActiveApplicationName;
- (NSString*) getTISPropertyInputModeID;

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

typedef enum {
  STATUSMESSAGETYPE_NONE,
  STATUSMESSAGETYPE_LOCK,
  STATUSMESSAGETYPE_EXTRA,
} StatusMessageType;
void set_statusmessage(StatusMessageType type, const char* message);

#ifdef __cplusplus
}
#endif

#endif

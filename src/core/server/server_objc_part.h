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
  NSImageView* statuswindow_backgroud_;

  NSString* statusmessage_lock_;
  NSString* statusmessage_extra_;
}

- (NSString*) getActiveApplicationName;
- (NSString*) getTISPropertyInputModeID;

- (void) registerStatusWindow:(NSWindow*)window label:(NSTextField*)label background:(NSImageView*)background;
- (void) updateStatusMessageWindow;
- (void) setStatusMessage:(StatusMessageType)type message:(const char*)message;
- (void) setStatusMessageWindowParam:(uint32_t)alpha_font alpha_background:(uint32_t)alpha_background;

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
void set_statusmessageWindowParam(uint32_t alpha_font, uint32_t alpha_background);

#ifdef __cplusplus
}
#endif

#endif

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
#import "ConfigXMLParser.h"
#import "PreferencesManager.h"

@interface ServerObjcPart : NSObject {
  NSWindow* statuswindow_;
  NSTextField* statuswindow_label_;
  NSImageView* statuswindow_backgroud_;

  NSString* statusmessage_lock_;
  NSString* statusmessage_extra_;

  IBOutlet ConfigXMLParser* configxmlparser_;
  IBOutlet PreferencesManager* preferencesmanager_;
}

- (NSArray*) getEssentialConfig;
- (NSUInteger) getConfigCount;
- (NSUInteger) getConfigInitializeVectorSize:(unsigned int)configindex;
- (NSArray*) getConfigInitializeVector:(unsigned int)configindex;
- (int) getConfigValue:(unsigned int)configindex;
- (void) selectInputSource:(unsigned int)vk_keycode;

- (NSString*) getActiveApplicationName;
- (NSString*) getTISPropertyInputModeID;

- (void) registerStatusWindow:(NSWindow*)window label:(NSTextField*)label background:(NSImageView*)background;

@end

// ------------------------------------------------------------
void registerServerObjcPart(ServerObjcPart* object);
void registerStatusWindow(NSWindow* window, NSTextField* label);

#endif

// ======================================================================
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @return errorcode
 */
int getEssentialConfig(int32_t* value, size_t len);

uint32_t getConfigCount(void);
uint32_t getConfigInitializeVectorSize(uint32_t configindex);
/**
 * @return errorcode
 */
int getConfigInitializeVector(uint32_t* value, size_t len, uint32_t configindex);

int getConfigValue(uint32_t configindex);

void selectInputSource(uint32_t vk_keycode);

void reset_statusmessage(void);
void set_statusmessage(StatusMessageType type, const char* message);
void set_statusmessageWindowParam(uint32_t alpha_font, uint32_t alpha_background, int32_t posx_adjustment, int32_t posy_adjustment);

#ifdef __cplusplus
}
#endif

#endif

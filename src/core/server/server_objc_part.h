/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#ifndef SERVER_OBJC_PART_H
#define SERVER_OBJC_PART_H

typedef enum {
  STATUSMESSAGETYPE_LOCK,
  STATUSMESSAGETYPE_EXTRA,
  STATUSMESSAGETYPE__END__,
} StatusMessageType;

#ifndef __cplusplus

#import <Cocoa/Cocoa.h>
#import "ConfigXMLParser.h"
#import "PreferencesManager.h"
#import "StatusWindow.h"

@interface ServerObjcPart : NSObject {
  IBOutlet ConfigXMLParser* configxmlparser_;
  IBOutlet PreferencesManager* preferencesmanager_;
  IBOutlet StatusWindow* statuswindow_;
}

- (NSArray*) getEssentialConfig;
- (NSUInteger) getConfigCount;
- (NSUInteger) getConfigInitializeVectorSize:(unsigned int)configindex;
- (NSArray*) getConfigInitializeVector:(unsigned int)configindex;
- (int) getConfigValue:(unsigned int)configindex;
- (void) selectInputSource:(unsigned int)vk_keycode;

- (void) setStatusMessage:(StatusMessageType)type message:(const char*)message;

- (NSString*) getActiveApplicationName;
- (NSString*) getTISPropertyInputModeID;

@end

// ------------------------------------------------------------
void registerServerObjcPart(ServerObjcPart* object);

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

void set_statusmessage(StatusMessageType type, const char* message);

#ifdef __cplusplus
}
#endif

#endif

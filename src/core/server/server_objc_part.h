/* -*- Mode: objc; Coding: utf-8; indent-tabs-mode: nil; -*- */

#ifndef SERVER_OBJC_PART_H
#define SERVER_OBJC_PART_H

#ifndef __cplusplus

#import <Cocoa/Cocoa.h>
#import "ConfigXMLParser.h"
#import "PreferencesManager.h"
#import "StatusWindow.h"
#import "WorkSpaceData.h"

@interface ServerObjcPart : NSObject {
  IBOutlet ConfigXMLParser* configxmlparser_;
  IBOutlet PreferencesManager* preferencesmanager_;
  IBOutlet StatusWindow* statuswindow_;
  IBOutlet WorkSpaceData* workspacedata_;
}

- (NSArray*) getEssentialConfig;
- (NSUInteger) getConfigCount;
- (NSUInteger) getConfigInitializeVectorSize:(unsigned int)configindex;
- (NSArray*) getConfigInitializeVector:(unsigned int)configindex;
- (int) getConfigValue:(unsigned int)configindex;

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

#ifdef __cplusplus
}
#endif

#endif

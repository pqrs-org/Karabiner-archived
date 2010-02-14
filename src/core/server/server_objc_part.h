#ifndef SERVER_OBJC_PART_H
#define SERVER_OBJC_PART_H

#ifndef __cplusplus

#import <Cocoa/Cocoa.h>

void getActiveApplicationName(char* buffer, size_t len);

// get current InputSourceProperty(kTISPropertyInputModeID)
//
// Note: getTISPropertyInputModeID works only by calling from "Application".
//       (== does not work from a terminal application).
//
void getTISPropertyInputModeID(char* buffer, size_t len);

// ------------------------------------------------------------
void registerStatusWindow(NSWindow* window, NSTextField* label_lock, NSTextField* label_extra);

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

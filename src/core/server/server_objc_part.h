#ifndef SERVER_OBJC_PART_H
#define SERVER_OBJC_PART_H

#ifndef __cplusplus

void getActiveApplicationName(char* buffer, size_t len);

// get current InputSourceProperty(kTISPropertyInputModeID)
//
// Note: getTISPropertyInputModeID works only by calling from "Application".
//       (== does not work from a terminal application).
//
void getTISPropertyInputModeID(char* buffer, size_t len);

#endif

// ======================================================================
#ifdef __cplusplus
extern "C" {
#endif

void selectInputSource_english(void);
void selectInputSource_french(void);
void selectInputSource_german(void);
void selectInputSource_japanese(void);

#ifdef __cplusplus
}
#endif

#endif

#ifndef SERVER_OBJC_PART_H
#define SERVER_OBJC_PART_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * These functions are not thread safe.
 */

void autoreleasepool_begin(void);
void autoreleasepool_end(void);

void getActiveApplicationName(char* buffer, size_t len);

/**
 * get current InputSourceProperty(kTISPropertyInputModeID)
 *
 * Note: getTISPropertyInputModeID works only by calling from "Application".
 *       (== does not work from a terminal application).
 */
void getTISPropertyInputModeID(char* buffer, size_t len);

#ifdef __cplusplus
}
#endif

#endif

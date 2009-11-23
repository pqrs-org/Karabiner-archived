#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * initialize server.
 * @return return 0 if failed.
 */
int server_initialize(const char* basedirectory);
void server_run(void);

void sysctl_reset(void);
void sysctl_load(void);

#ifdef __cplusplus
}
#endif

#endif

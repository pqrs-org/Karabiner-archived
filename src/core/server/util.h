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

void set_sysctl_do_reload_xml(void);
void set_sysctl_do_reset(void);
void set_sysctl_do_reload_only_config(void);

#ifdef __cplusplus
}
#endif

#endif

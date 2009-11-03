#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * check the process user is a login user.
 *
 * @return return 0 if invalid user.
 */
int verifyUser(void);
void runServer(void);

#ifdef __cplusplus
}
#endif

#endif

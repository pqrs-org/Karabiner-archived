#include <sys/systm.h>

#include "remap.hpp"
#include "RemapUtil.hpp"
#include "KeyboardRepeat.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "IOLockWrapper.hpp"
#include "KeyCode.hpp"
#include "ListHookedKeyboard.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace GeneratedCode {
#include "config/output/include.remapcode_func.cpp"
  }
}

// ----------------------------------------------------------------------
namespace org_pqrs_KeyRemap4MacBook {
  namespace {
#include "config/output/include.remapcode_info.cpp"

    typedef void (*RemapFunc_key)(RemapParams& remapParams);
    RemapFunc_key listRemapFunc_key[MAXNUM_REMAPFUNC_KEY + 1];
    int listRemapFunc_key_size = 0;

    typedef void (*RemapFunc_consumer)(RemapConsumerParams& remapParams);
    RemapFunc_consumer listRemapFunc_consumer[MAXNUM_REMAPFUNC_CONSUMER + 1];
    int listRemapFunc_consumer_size = 0;

    typedef void (*RemapFunc_pointing)(RemapPointingParams_relative& remapParams);
    RemapFunc_pointing listRemapFunc_pointing[MAXNUM_REMAPFUNC_POINTING + 1];
    int listRemapFunc_pointing_size = 0;

    IOLock* refresh_remapfunc_lock = NULL;
  }
}

void
org_pqrs_KeyRemap4MacBook::remapfunc_initialize(void)
{
  refresh_remapfunc_lock = IOLockWrapper::alloc();

  refresh_remapfunc();
}

void
org_pqrs_KeyRemap4MacBook::remapfunc_terminate(void)
{
  IOLockWrapper::free(refresh_remapfunc_lock);
}

void
org_pqrs_KeyRemap4MacBook::refresh_remapfunc(void)
{
  IOLockWrapper::ScopedLock lk(refresh_remapfunc_lock);

  KeyboardRepeat::cancel();

  listRemapFunc_key_size = 0;
  listRemapFunc_consumer_size = 0;
  listRemapFunc_pointing_size = 0;

  // ------------------------------------------------------------
#include "config/output/include.remapcode_refresh_remapfunc_key.cpp"

  // ------------------------------------------------------------
#include "config/output/include.remapcode_refresh_remapfunc_consumer.cpp"

  // ------------------------------------------------------------
#include "config/output/include.remapcode_refresh_remapfunc_pointing.cpp"

  // ------------------------------------------------------------
  // handle StatusMessage
  static char statusmessage[32];
  static char lastmessage[32];
  statusmessage[0] = '\0';
  bool isStatusMessageVisible = false;
#include "config/output/include.remapcode_refresh_remapfunc_statusmessage.cpp"

  if (strcmp(statusmessage, lastmessage) != 0) {
    if (! isStatusMessageVisible) {
      // hide
      KeyRemap4MacBook_bridge::StatusMessage::Request request(KeyRemap4MacBook_bridge::StatusMessage::MESSAGETYPE_EXTRA, "");
      KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_STATUS_MESSAGE, &request, sizeof(request), NULL, 0);
    } else {
      // show
      KeyRemap4MacBook_bridge::StatusMessage::Request request(KeyRemap4MacBook_bridge::StatusMessage::MESSAGETYPE_EXTRA, statusmessage);
      KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_STATUS_MESSAGE, &request, sizeof(request), NULL, 0);
    }
  }
  strlcpy(lastmessage, statusmessage, sizeof(lastmessage));

  // ------------------------------------------------------------
  if (config.debug_devel) {
    printf("KeyRemap4MacBook --INFO-- enabled remappings: key:%d, consumer:%d, pointing:%d\n",
           listRemapFunc_key_size,
           listRemapFunc_consumer_size,
           listRemapFunc_pointing_size);
  }
}

void
org_pqrs_KeyRemap4MacBook::remap_core(RemapParams& remapParams)
{
  for (int i = 0; i < listRemapFunc_key_size; ++i) {
    RemapFunc_key func = listRemapFunc_key[i];
    if (func) {
      func(remapParams);
    }
  }
}

void
org_pqrs_KeyRemap4MacBook::remap_consumer(RemapConsumerParams& remapParams)
{
  for (int i = 0; i < listRemapFunc_consumer_size; ++i) {
    RemapFunc_consumer func = listRemapFunc_consumer[i];
    if (func) {
      func(remapParams);
    }
  }
}

void
org_pqrs_KeyRemap4MacBook::remap_pointing_relative_core(RemapPointingParams_relative& remapParams)
{
  for (int i = 0; i < listRemapFunc_pointing_size; ++i) {
    RemapFunc_pointing func = listRemapFunc_pointing[i];
    if (func) {
      func(remapParams);
    }
  }
}

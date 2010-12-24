#ifndef BRIDGE_HPP
#define BRIDGE_HPP

#include <string.h>
#include "bridge.h"

namespace org_pqrs_KeyRemap4MacBook {
  namespace KeyRemap4MacBook_bridge {
    enum Error {
      SUCCESS = 0,
      ERROR = 1,
    };

    enum RequestType {
      REQUEST_NONE,
      REQUEST_GET_ESSENTIAL_CONFIG,
      REQUEST_GET_CONFIG_COUNT,
      REQUEST_GET_CONFIG_INFO,
      REQUEST_GET_CONFIG_INITIALIZE_VECTOR,
      REQUEST_GET_WORKSPACE_DATA,
      REQUEST_CHANGE_INPUTMODE,
      REQUEST_STATUS_MESSAGE,
      REQUEST_STATUS_MESSAGE_WINDOW_PARAMETER,
    };

    namespace GetEssentialConfig {
      // none
      struct Request {};

      struct Reply {
        int32_t value[BRIDGE_ESSENTIAL_CONFIG_INDEX__END__];
      };
    }

    namespace GetConfigCount {
      // none
      struct Request {};

      struct Reply {
        uint32_t count;
      };
    }

    namespace GetConfigInfo {
      // none
      struct Request {};

      struct Reply {
        struct Item {
          uint32_t initialize_vector_size;
          uint32_t enabled;
        } items[0];
      };
    }

    namespace GetConfigInitializeVector {
      struct Request {
        Request(uint32_t c) : configindex(c) {}
        uint32_t configindex;
      };

      struct Reply {
        uint32_t initialize_vector[0];
      };
    }

    namespace GetWorkspaceData {
      // none
      struct Request {};

      struct Reply {
        uint32_t type;
        uint32_t inputmode;
        uint32_t inputmodedetail;
      };
    }

    namespace ChangeInputMode {
      struct Request {
        uint32_t vk_keycode;
      };
      struct Reply {};
    }

    namespace StatusMessage {
      enum MessageType {
        MESSAGETYPE_NONE,
        MESSAGETYPE_MODIFIER_LOCK,
        MESSAGETYPE_EXTRA,
      };

      struct Request {
        Request(void) : type(MESSAGETYPE_NONE) {
          message[0] = '\0';
        }

        Request(MessageType t, const char* m = "") : type(t) {
          if (! m) {
            message[0] = '\0';
          } else {
            strlcpy(message, m, sizeof(message));
          }
        }

        MessageType type;
        char message[16];
      };
      struct Reply {};
    }
  }
}

#endif

#ifndef BRIDGE_HPP
#define BRIDGE_HPP

#include <string.h>

namespace org_pqrs_KeyRemap4MacBook {
  namespace KeyRemap4MacBook_bridge {
    enum Error {
      SUCCESS = 0,
      ERROR = 1,
    };

    enum RequestType {
      REQUEST_NONE,
      REQUEST_GET_CONFIG_COUNT,
      REQUEST_GET_CONFIG_INFO,
      REQUEST_GET_CONFIG_INITIALIZE_VECTOR,
      REQUEST_GET_WORKSPACE_DATA,
      REQUEST_CHANGE_INPUTMODE,
      REQUEST_STATUS_MESSAGE,
      REQUEST_STATUS_MESSAGE_WINDOW_PARAMETER,
    };

    namespace GetConfigCount {
      // none
      struct Request {};

      struct Reply {
        uint32_t count;
      };
    }

    namespace GetConfigInfo {
      struct Request {
        Request(uint32_t c) : count(c) {}
        uint32_t count;
      };

      struct Reply {
        struct Item {
          uint32_t initialize_vector_size;
          uint32_t enabled;
        } items[0];
      };
    }

    namespace GetConfigInitializeVector {
      struct Request {
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
      enum InputMode {
        INPUTMODE_ENGLISH,
        INPUTMODE_FRENCH,
        INPUTMODE_GERMAN,
        INPUTMODE_JAPANESE,
        INPUTMODE_SWEDISH,
        INPUTMODE_CANADIAN,
        INPUTMODE_RUSSIAN,
        INPUTMODE_RUSSIAN_TYPOGRAPHIC,
        INPUTMODE_ENGLISH_TYPOGRAPHIC,
      };
      struct Request {
        InputMode inputmode;
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

    namespace StatusMessageWindowParameter {
      struct Request {
        Request(void) : alpha_font(0), alpha_background(0) {}
        Request(uint32_t af, uint32_t ab, int32_t px, int32_t py) : alpha_font(af), alpha_background(ab), posx_adjustment(px), posy_adjustment(py) {}

        uint32_t alpha_font;
        uint32_t alpha_background;
        int32_t posx_adjustment;
        int32_t posy_adjustment;
      };
      struct Reply {};
    }
  }
}

#endif

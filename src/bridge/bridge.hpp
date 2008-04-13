#ifndef KEYREMAP4MACBOOK_SERVER_H
#define KEYREMAP4MACBOOK_SERVER_H

namespace KeyRemap4MacBook_server {
  namespace Bridge {
    const char * const socketPath = "/Library/org.pqrs/KeyRemap4MacBook/tmp/socket";

    enum RequestType {
      REQUEST_TYPE_NONE,
      REQUEST_TYPE_ACTIVE_APPLICATION_INFO,
    };

    enum ReplyType {
      REPLYTYPE_SUCCESS = 0,
      REPLYTYPE_ERROR = 1,
    };

    namespace ActiveApplicationInfo {
      struct Reply {
        bool is_emacs; // Carbon Emacs, ...
        bool is_terminal; // Terminal.app, iTerm.app, ...
        bool is_virtualmachine; // VMware, Parallels, ...
      };
    }
  }
}

#endif

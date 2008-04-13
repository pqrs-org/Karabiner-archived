#ifndef BRIDGE_HPP
#define BRIDGE_HPP

namespace KeyRemap4MacBook_bridge {
  const char * const socketPath = "/Library/org.pqrs/KeyRemap4MacBook/tmp/socket";

  enum Error {
    SUCCESS = 0,
    ERROR = 1,
  };

  enum RequestType {
    REQUEST_NONE,
    REQUEST_ACTIVE_APPLICATION_INFO,
  };

  namespace ActiveApplicationInfo {
    // none
    struct Request {
    };

    struct Reply {
      bool is_emacs; // Carbon Emacs, ...
      bool is_terminal; // Terminal.app, iTerm.app, ...
      bool is_virtualmachine; // VMware, Parallels, ...

      void reset(void) {
        is_emacs = false;
        is_terminal = false;
        is_virtualmachine = false;
      }
    };
  }
}

#endif

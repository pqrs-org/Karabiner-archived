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
    enum ApplicationType {
      UNKNOWN,
      EMACS, // Carbon Emacs, Aquamacs, ...
      VI, // Vim, ...
      TERMINAL, // Terminal.app, iTerm.app, ...
      VIRTUALMACHINE, // VMware, Parallels, ...
      REMOTEDESKTOPCONNECTION, // Microsoft Remote Desktop Connection, Cord, ...
      X11,
      FINDER,
      SAFARI,
      FIREFOX,
      ICHAT, // iChat
      ADIUMX,
      SKYPE,
      MAIL, // Mail.app
      EDITOR, // TextEdit
    };

    // none
    struct Request {
    };

    struct Reply {
      ApplicationType type;
    };
  }
}

#endif

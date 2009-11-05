#ifndef BRIDGE_HPP
#define BRIDGE_HPP

namespace org_pqrs_KeyRemap4MacBook {
  namespace KeyRemap4MacBook_bridge {
    enum Error {
      SUCCESS = 0,
      ERROR = 1,
    };

    enum RequestType {
      REQUEST_NONE,
      REQUEST_ACTIVE_APPLICATION_INFO,
    };

    namespace GetWorkspaceData {
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
        ADOBE, // Adobe Softwares
        THUNDERBIRD,
        EXCEL, // Microsoft Excel
        ENTOURAGE, // Microsoft Entourage
        ECLIPSE,
      };
      enum InputMode {
        INPUTMODE_UNKNOWN,
        INPUTMODE_ROMAN, // Roman, Password
        INPUTMODE_JAPANESE, // Japanese, Japanese.*
      };

      // none
      struct Request {
      };

      struct Reply {
        ApplicationType type;
        InputMode inputmode;
      };
    }
  }
}

#endif

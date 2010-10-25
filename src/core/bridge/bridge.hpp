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
      REQUEST_GET_WORKSPACE_DATA,
      REQUEST_CHANGE_INPUTMODE,
      REQUEST_STATUS_MESSAGE,
      REQUEST_STATUS_MESSAGE_WINDOW_PARAMETER,
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
        PHOTOSHOP,
        ILLUSTRATOR,
        ADOBE, // Adobe Softwares
        THUNDERBIRD,
        EXCEL, // Microsoft Excel
        ENTOURAGE, // Microsoft Entourage
        ECLIPSE,
        MACSOUP,
        VIENNA,
        XCODE,
        OPERA,
        GOOGLE_CHROME,
        TEAMVIEWER,
        CHEETAH_3D,
        PASSWORD, // com.apple.securityagent
        GRUML,
        IPHOTO,
        STICKIES,
        LAUNCHBAR,
        WEBIDE, // PhpStorm, WebStorm
      };
      enum InputMode {
        INPUTMODE_NONE,
        INPUTMODE_ROMAN, // Roman, Password, ...
        INPUTMODE_JAPANESE, // Japanese, Japanese.*
        INPUTMODE_CHINESE_TRADITIONAL,
        INPUTMODE_CHINESE_SIMPLIFIED,
        INPUTMODE_KOREAN,
        INPUTMODE_SWEDISH,
        INPUTMODE_CANADIAN,
        INPUTMODE_AINU,
        INPUTMODE_RUSSIAN,
        INPUTMODE_FRENCH,
        INPUTMODE_BEPO, // http://bepo.fr/wiki/Mac_OS_X
        INPUTMODE_DVORAK,
        INPUTMODE_RUSSIAN_TYPOGRAPHIC, // http://ilyabirman.ru/typography-layout/
        INPUTMODE_ENGLISH_TYPOGRAPHIC, // http://ilyabirman.ru/typography-layout/
        INPUTMODE_UNKNOWN, // User Defined keylayout (example: org.unknown.keylayout.JANSI)
      };
      enum InputModeDetail {
        INPUTMODE_DETAIL_NONE,

        INPUTMODE_DETAIL_ROMAN,
        INPUTMODE_DETAIL_ROMAN_JANSI, // http://homepage.mac.com/kenie/Projects/JANSI/

        INPUTMODE_DETAIL_JAPANESE_HIRAGANA,
        INPUTMODE_DETAIL_JAPANESE_KATAKANA,
        INPUTMODE_DETAIL_JAPANESE_FULLWIDTH_ROMAN,
        INPUTMODE_DETAIL_JAPANESE_HALFWIDTH_KANA,
        INPUTMODE_DETAIL_JAPANESE, // other Japanese mode

        INPUTMODE_DETAIL_CHINESE_TRADITIONAL,
        INPUTMODE_DETAIL_CHINESE_SIMPLIFIED,

        INPUTMODE_DETAIL_KOREAN,
        INPUTMODE_DETAIL_SWEDISH,
        INPUTMODE_DETAIL_CANADIAN,
        INPUTMODE_DETAIL_AINU,
        INPUTMODE_DETAIL_RUSSIAN,
        INPUTMODE_DETAIL_FRENCH,

        INPUTMODE_DETAIL_BEPO,
        INPUTMODE_DETAIL_BEPO_AZERTYCMD,
        INPUTMODE_DETAIL_BEPO_AZERTYCMDROMAN,
        INPUTMODE_DETAIL_BEPO_QWERTYCMD,
        INPUTMODE_DETAIL_BEPO_QWERTYCMDROMAN,
        INPUTMODE_DETAIL_BEPO_QWERTZCMD,
        INPUTMODE_DETAIL_BEPO_QWERTZCMDROMAN,
        INPUTMODE_DETAIL_BEPO_ROMAN,

        INPUTMODE_DETAIL_DVORAK,
        INPUTMODE_DETAIL_DVORAK_LEFT,
        INPUTMODE_DETAIL_DVORAK_QWERTYCMD,
        INPUTMODE_DETAIL_DVORAK_RIGHT,

        INPUTMODE_DETAIL_RUSSIAN_TYPOGRAPHIC,
        INPUTMODE_DETAIL_ENGLISH_TYPOGRAPHIC,

        INPUTMODE_DETAIL_UNKNOWN,
      };

      // none
      struct Request {};

      struct Reply {
        ApplicationType type;
        InputMode inputmode;
        InputModeDetail inputmodedetail;
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

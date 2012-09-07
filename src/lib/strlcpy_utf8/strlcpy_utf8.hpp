#ifndef STRLCPY_UTF8_HPP
#define STRLCPY_UTF8_HPP

namespace pqrs {
  class strlcpy_utf8 {
  public:
    static void
    strlcpy(char* dst, const char* src, size_t size)
    {
      ::strlcpy(dst, src, size);

      size_t len = strlen(dst);

      // removing an incomplete multi-byte character at tail.
      size_t i = 0;
      size_t previous = 0;
      while (true) {
        previous = i;

        if ((dst[i] & 0x80) == 0) { // 0xxxxxxx
          ++i;
        } else if ((dst[i] & 0xe0) == 0xc0) { // 110xxxxx
          i += 2;
        } else if ((dst[i] & 0xf0) == 0xe0) { // 1110xxxx
          i += 3;
        } else if ((dst[i] & 0xf8) == 0xf0) { // 11110xxx
          i += 4;
        }

        if (i > len) {
          break;
        }
      }
      dst[previous] = '\0';
    }
  };
}

#endif

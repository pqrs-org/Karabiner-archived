#ifndef STRLCPY_UTF8_HPP
#define STRLCPY_UTF8_HPP

namespace pqrs {
class strlcpy_utf8 final {
public:
  static size_t
  strlcpy(char* dst, const char* src, size_t size) {
    if (dst == nullptr) return -1;
    if (src == nullptr) return -1;
    if (size == 0) return -1;

    size_t retval = ::strlcpy(dst, src, size);

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
      } else {
        break;
      }

      if (i > len) {
        break;
      }
    }
    dst[previous] = '\0';

    return retval;
  }
};
}

#endif

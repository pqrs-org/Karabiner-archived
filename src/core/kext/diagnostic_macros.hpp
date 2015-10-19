#ifndef DIAGNOSTIC_MACROS_HPP
#define DIAGNOSTIC_MACROS_HPP

#define BEGIN_IOKIT_INCLUDE        \
  _Pragma("clang diagnostic push") \
      _Pragma("clang diagnostic ignored \"-Winconsistent-missing-override\"")

#define END_IOKIT_INCLUDE \
  _Pragma("clang diagnostic pop")

#endif

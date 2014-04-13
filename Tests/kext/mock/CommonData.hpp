#ifndef COMMONDATA_HPP
#define COMMONDATA_HPP

class CommonData {
public:
  static void increase_alloccount(void) {}
  static void decrease_alloccount(void) {}

  static void clear_statusmessage(int index) {}
  static void append_statusmessage(int index, const char* message) {}
  static void send_notification_statusmessage(int index) {}
  static const char* get_statusmessage(int index) { return ""; }
};

#endif

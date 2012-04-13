// This header intentionally has no include guards.

class error_information {
public:
  error_information(void) : count_(0) {}

  const std::string& get_message(void) const { return message_; }
  size_t get_count(void)               const { return count_; }

  void set(const std::string& message) {
    if (message_.empty()) {
      message_ = message;
    }
    ++count_;
  }
  void set(const boost::format& message) {
    set(message.str());
  }

  void clear(void) {
    message_.clear();
    count_ = 0;
  }

private:
  std::string message_;
  size_t count_;
};

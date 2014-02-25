#ifndef DELTABUFFER_HPP
#define DELTABUFFER_HPP

namespace org_pqrs_KeyRemap4MacBook {
  // DeltaBuffer stores dx and dy of RelativePointerEvent in the short term
  // in order to calculate an average of pointer movement.

  class DeltaBuffer {
  public:
    DeltaBuffer(void) {
      clear();
    }

    void clear(void) {
      for (int i = 0; i < BUFFER_LENGTH; ++i) {
        buffer_[i] = 0;
      }
      count_ = 0;
      sign_ = 0;
    }

    void push(int newval) {
      // if newval's sign and stored data's sign are different,
      // clear the stored data because the direction of pointer movement is changed.
      if ((newval > 0 && sign_ < 0) ||
          (newval < 0 && sign_ > 0)) {
        clear();
      }

      for (int i = 0; i < BUFFER_LENGTH - 1; ++i) {
        buffer_[i] = buffer_[i + 1];
      }
      buffer_[BUFFER_LENGTH - 1] = newval;

      if (count_ < BUFFER_LENGTH) {
        ++count_;
      }

      if (newval > 0) {
        sign_ = 1;
      }
      if (newval < 0) {
        sign_ = -1;
      }
    }

    bool isFull(void) const {
      return (count_ >= BUFFER_LENGTH);
    }

    int sum(void) const {
      int s = 0;
      for (int i = 0; i < BUFFER_LENGTH; ++i) {
        s += buffer_[i];
      }
      return s;
    }

  private:
    enum {
      BUFFER_LENGTH = 5,
    };

    int buffer_[BUFFER_LENGTH];
    size_t count_;
    int sign_;
  };
}

#endif

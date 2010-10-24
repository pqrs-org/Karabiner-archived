#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "base.hpp"

#define DECLARE_VECTOR(TYPENAME)                                      \
  class Vector_ ## TYPENAME {                                         \
  public:                                                             \
    Vector_ ## TYPENAME(void) : vector_(NULL), size_(0) {}            \
    ~Vector_ ## TYPENAME(void) {                                      \
      if (vector_) {                                                  \
        delete[] vector_;                                             \
        vector_ = NULL;                                               \
      }                                                               \
    }                                                                 \
                                                                      \
    Vector_ ## TYPENAME & push_back(const TYPENAME &newval) {         \
      TYPENAME* p = new TYPENAME[size_ + 1];                          \
      if (p) {                                                        \
        for (size_t i = 0; i < size_; ++i) {                          \
          p[i] = vector_[i];                                          \
        }                                                             \
        p[size_] = newval;                                            \
      }                                                               \
      ++size_;                                                        \
                                                                      \
      if (vector_) {                                                  \
        delete[] vector_;                                             \
      }                                                               \
                                                                      \
      vector_ = p;                                                    \
                                                                      \
      return *this;                                                   \
    }                                                                 \
                                                                      \
    size_t size(void) const { return size_; }                         \
    bool empty(void) const { return size_ == 0; }                     \
                                                                      \
    TYPENAME & operator[](size_t n) { return vector_[n]; }            \
    const TYPENAME& operator[](size_t n) const { return vector_[n]; } \
    TYPENAME& front(void) { return vector_[0]; }                      \
    TYPENAME& back(void) { return vector_[size_ - 1]; }               \
                                                                      \
  private:                                                            \
    TYPENAME* vector_;                                                \
    size_t size_;                                                     \
  };

#endif

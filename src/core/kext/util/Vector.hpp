#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "base.hpp"

#define DECLARE_VECTOR(TYPENAME)                                         \
  class Vector_ ## TYPENAME {                                            \
  public:                                                                \
    Vector_ ## TYPENAME(void) : vector_(NULL), capacity_(0), size_(0) {} \
    ~Vector_ ## TYPENAME(void) {                                         \
      if (vector_) {                                                     \
        delete[] vector_;                                                \
      }                                                                  \
    }                                                                    \
                                                                         \
    void reserve(size_t n) {                                             \
      if (n <= capacity_) return;                                        \
                                                                         \
      TYPENAME* p = new TYPENAME[n];                                     \
      if (p) {                                                           \
        capacity_ = n;                                                   \
        for (size_t i = 0; i < size_; ++i) {                             \
          p[i] = vector_[i];                                             \
        }                                                                \
                                                                         \
        if (vector_) {                                                   \
          delete[] vector_;                                              \
        }                                                                \
        vector_ = p;                                                     \
      }                                                                  \
    }                                                                    \
                                                                         \
    void clear(void) {                                                   \
      if (vector_) {                                                     \
        delete[] vector_;                                                \
        vector_ = NULL;                                                  \
      }                                                                  \
      capacity_ = 0;                                                     \
      size_ = 0;                                                         \
    }                                                                    \
                                                                         \
    Vector_ ## TYPENAME & push_back(const TYPENAME &newval) {            \
      if (size_ >= capacity_) {                                          \
        reserve(capacity_ + 1);                                          \
      }                                                                  \
                                                                         \
      if (size_ < capacity_) {                                           \
        vector_[size_] = newval;                                         \
        ++size_;                                                         \
      }                                                                  \
                                                                         \
      return *this;                                                      \
    }                                                                    \
                                                                         \
    size_t size(void) const { return size_; }                            \
    size_t capacity(void) const { return capacity_; }                    \
    bool empty(void) const { return size_ == 0; }                        \
                                                                         \
    TYPENAME& operator[](size_t n) { return vector_[n]; }                \
    const TYPENAME& operator[](size_t n) const { return vector_[n]; }    \
    TYPENAME& front(void) { return vector_[0]; }                         \
    TYPENAME& back(void) { return vector_[size_ - 1]; }                  \
                                                                         \
  private:                                                               \
    TYPENAME* vector_;                                                   \
    size_t capacity_;                                                    \
    size_t size_;                                                        \
  };

#endif

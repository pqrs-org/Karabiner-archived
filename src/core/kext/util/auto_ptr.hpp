#ifndef AUTO_PTR_HPP
#define AUTO_PTR_HPP

#define DECLARE_AUTO_PTR(CLASS)                         \
  class auto_ptr {                                      \
  public:                                               \
    auto_ptr(CLASS * ptr) { ptr_ = ptr; }               \
    ~auto_ptr(void) { if (ptr_) delete ptr_; }          \
    bool operator!(void) const { return ptr_ == NULL; } \
    CLASS & operator*(void) const { return *ptr_; }     \
                                                        \
  private:                                              \
    auto_ptr(auto_ptr & ptr);                           \
    CLASS* ptr_;                                        \
  };

#endif

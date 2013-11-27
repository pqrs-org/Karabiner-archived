#include "List.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  List::List(void) : front_(NULL), back_(NULL), size_(0)
  {}

  List::~List(void)
  {
    clear();
  }

  List::Item*
  List::erase(Item* p)
  {
    if (! p) return NULL;

    Item* next = p->next_;

    if (p->prev_) {
      p->prev_->next_ = p->next_;
    }
    if (p->next_) {
      p->next_->prev_ = p->prev_;
    }
    if (front_ == p) {
      front_ = p->next_;
    }
    if (back_ == p) {
      back_ = p->prev_;
    }

    --size_;

    return next;
  }

  List::Item*
  List::erase_and_delete(Item* p)
  {
    if (! p) return NULL;

    Item* next = erase(p);
    delete p;
    return next;
  }

  void
  List::clear(void)
  {
    while (front_) {
      erase_and_delete(front_);
    }
  }

  void
  List::push_back(Item* p)
  {
    if (! p) return;

    if (back_) {
      back_->next_ = p;
    }
    p->prev_ = back_;
    p->next_ = NULL;

    back_ = p;

    if (! front_) {
      front_ = back_;
    }

    ++size_;
  }

  void
  List::push_front(Item* p)
  {
    if (! p) return;

    if (front_) {
      front_->prev_ = p;
    }
    p->prev_ = NULL;
    p->next_ = front_;

    front_ = p;

    if (! back_) {
      back_ = front_;
    }

    ++size_;
  }
}

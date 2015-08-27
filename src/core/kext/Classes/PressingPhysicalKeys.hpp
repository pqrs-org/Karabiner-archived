#ifndef PRESSINGPHYSICALKEYS_HPP
#define PRESSINGPHYSICALKEYS_HPP

#include "FromEvent.hpp"
#include "List.hpp"

namespace org_pqrs_Karabiner {
class PressingPhysicalKeys final {
public:
  void update(const Params_Base& paramsBase);
  void clear(void);
  bool empty(void) const;
  size_t count(void) const;

private:
  class Item final : public List::Item {
  public:
    Item(const Params_Base& paramsBase) : fromEvent(paramsBase) {}
    virtual ~Item(void) {}

    FromEvent fromEvent;
  };
  List list_;
};
}

#endif

#ifndef PRESSINGPHYSICALKEYS_HPP
#define PRESSINGPHYSICALKEYS_HPP

#include "FromEvent.hpp"
#include "List.hpp"

namespace org_pqrs_Karabiner {
class PressingPhysicalKeys final {
public:
  static void initialize(void);
  static void terminate(void);
  static void update(const Params_Base& paramsBase);
  static void clear(void);
  static bool empty(void);
  static size_t count(void);

private:
  class Item final : public List::Item {
  public:
    Item(const Params_Base& paramsBase) : fromEvent(paramsBase) {}
    virtual ~Item(void) {}

    FromEvent fromEvent;
  };
  static List list_;
};
}

#endif

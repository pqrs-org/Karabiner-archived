#ifndef PRESSINGPHYSICALKEYS_HPP
#define PRESSINGPHYSICALKEYS_HPP

namespace org_pqrs_Karabiner {
class PressingPhysicalKeys final {
public:
  static void set(int diff);
  static void reset(void);
  static bool iszero(void);

private:
  static void log(void);

  static int num_;
};
}

#endif

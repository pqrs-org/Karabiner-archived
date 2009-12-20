#include "CommonData.hpp"
#include "IOLockWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  AbsoluteTime CommonData::current_ts_;
  KeyboardType CommonData::current_keyboardType_;
  IOLock* CommonData::eventLock = NULL;

  bool
  CommonData::initialize(void)
  {
    eventLock = IOLockWrapper::alloc();
    if (! eventLock) return false;

    return true;
  }

  void
  CommonData::terminate(void)
  {
    IOLockWrapper::free(eventLock);
  }
}

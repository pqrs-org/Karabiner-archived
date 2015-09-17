#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOService.h>
END_IOKIT_INCLUDE;

// http://developer.apple.com/documentation/Darwin/Conceptual/KEXTConcept/KEXTConceptIOKit/hello_iokit.html#//apple_ref/doc/uid/20002366-CIHECHHE
class org_pqrs_driver_Karabiner final : public IOService {
  OSDeclareDefaultStructors(org_pqrs_driver_Karabiner);

public:
  virtual bool init(OSDictionary* dictionary = 0) override;
  virtual void free(void) override;
  virtual IOService* probe(IOService* provider, SInt32* score) override;
  virtual bool start(IOService* provider) override;
  virtual void stop(IOService* provider) override;

private:
  bool initialize_notification(void);
  void terminate_notification(void);

  IONotifier* notifier_hookKeyboard_;
  IONotifier* notifier_unhookKeyboard_;

  IONotifier* notifier_hookPointing_;
  IONotifier* notifier_unhookPointing_;
};

#endif

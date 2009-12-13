#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "base.hpp"

// http://developer.apple.com/documentation/Darwin/Conceptual/KEXTConcept/KEXTConceptIOKit/hello_iokit.html#//apple_ref/doc/uid/20002366-CIHECHHE
class org_pqrs_driver_KeyRemap4MacBook : public IOService
{
  OSDeclareDefaultStructors(org_pqrs_driver_KeyRemap4MacBook);

public:
  virtual bool init(OSDictionary* dictionary = 0);
  virtual void free(void);
  virtual IOService* probe(IOService* provider, SInt32* score);
  virtual bool start(IOService* provider);
  virtual void stop(IOService* provider);

private:
  bool initialize_notification(void);
  void terminate_notification(void);

  IONotifier* notifier_hookKeyboard_;
  IONotifier* notifier_unhookKeyboard_;

  IONotifier* notifier_hookPointing_;
  IONotifier* notifier_unhookPointing_;
};

#endif

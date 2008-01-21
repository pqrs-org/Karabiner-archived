// -*- indent-tabs-mode: nil; -*-

#ifndef _KeyRemap4MacBook_h
#define _KeyRemap4MacBook_h

// http://developer.apple.com/documentation/Darwin/Conceptual/KEXTConcept/KEXTConceptIOKit/hello_iokit.html#//apple_ref/doc/uid/20002366-CIHECHHE
class org_pqrs_driver_KeyRemap4MacBook : public IOService
{
  OSDeclareDefaultStructors(org_pqrs_driver_KeyRemap4MacBook);

public:
  virtual bool init(OSDictionary *dictionary = 0);
  virtual void free(void);
  virtual IOService *probe(IOService *provider, SInt32 *score);
  virtual bool start(IOService *provider);
  virtual void stop(IOService *provider);

private:
  enum {
    MAXNUM_KEYBOARD = 4,
  };

  // ------------------------------------------------------------
  struct HookedKeyboard {
    IOHIKeyboard *kbd;
    KeyboardEventAction origEventAction;

    // for keyrepeat
    IOTimerEventSource *timer;
    struct RepeatInfo {
      unsigned int flags;
      unsigned int key;
      unsigned int charCode;
      unsigned int charSet;
      unsigned int origCharCode;
      unsigned int origCharSet;
      unsigned int keyboardType;
      AbsoluteTime ts;
      OSObject *target;
    } repeat;

    void initialize(IOHIKeyboard *_kbd, IOWorkLoop *workLoop);
    void terminate(IOWorkLoop *workLoop);
  };
  static HookedKeyboard hookedKeyboard[MAXNUM_KEYBOARD];
  static HookedKeyboard *new_hookedKeyboard(void);
  static HookedKeyboard *search_hookedKeyboard(const IOHIKeyboard *kbd);

  static bool notifier_hookKeyboard(org_pqrs_driver_KeyRemap4MacBook *self, void *ref, IOService *newService);
  static bool notifier_unhookKeyboard(org_pqrs_driver_KeyRemap4MacBook *self, void *ref, IOService *newService);

  static bool replaceKeyboardEvent(org_pqrs_driver_KeyRemap4MacBook *self, IOHIKeyboard *kbd);

  IONotifier *keyboardNotifier;
  IONotifier *terminatedNotifier;

  // ------------------------------------------------------------
  static void keyboardEventCallBack(OSObject *target,
                                    unsigned int eventType,
                                    unsigned int flags,
                                    unsigned int key,
                                    unsigned int charCode,
                                    unsigned int charSet,
                                    unsigned int origCharCode,
                                    unsigned int origCharSet,
                                    unsigned int keyboardType,
                                    bool repeat,
                                    AbsoluteTime ts,
                                    OSObject *sender,
                                    void *refcon);

  static void doKeyRepeat(OSObject *owner, IOTimerEventSource *sender);

  IOWorkLoop *workLoop;
};

#endif

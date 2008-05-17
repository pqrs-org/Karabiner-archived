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
    MAXNUM_CONSUMER = 4,
    MAXNUM_POINTING = 4,
  };

  class TimerWrapper {
  public:
    TimerWrapper(void) { workLoop = NULL; timer = NULL; }
    void initialize(IOWorkLoop *_workLoop, OSObject *owner, IOTimerEventSource::Action func);
    void terminate(void);

    void setTimeoutMS(int interval) { if (timer) timer->setTimeoutMS(interval); }
    void cancelTimeout(void) { if (timer) timer->cancelTimeout(); }

  private:
    IOWorkLoop *workLoop;
    IOTimerEventSource *timer;
  };

  IOWorkLoop *workLoop;
  TimerWrapper timer_refresh;

  // ------------------------------------------------------------
  struct HookedKeyboard {
    IOHIKeyboard *kbd;
    KeyboardEventCallback origEventCallback;
    // save EventTarget for consumerToKey
    OSObject *origEventTarget;

    // for keyrepeat
    TimerWrapper timer_repeat;
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
      OSObject *sender;
      void *refcon;
    } repeat;

    TimerWrapper timer_extraRepeat;

    void initialize(IOHIKeyboard *_kbd, IOWorkLoop *workLoop);
    void refresh(void);
    void terminate(IOWorkLoop *workLoop);
    void setRepeatInfo(unsigned int eventType,
                       unsigned int flags, unsigned int key,
                       unsigned int charCode, unsigned int charSet, unsigned int origCharCode, unsigned int origCharSet,
                       unsigned int keyboardType, AbsoluteTime ts,
                       OSObject *target, void *refcon);
  };
  static HookedKeyboard hookedKeyboard[MAXNUM_KEYBOARD];
  static HookedKeyboard *new_hookedKeyboard(void);
  static HookedKeyboard *search_hookedKeyboard(const IOHIKeyboard *kbd);
  static HookedKeyboard *get_1stHookedKeyboard(void);
  static KeyboardEventCallback getKeyboardEventCallback(IOHIKeyboard *kbd) {
    return reinterpret_cast<KeyboardEventCallback>(kbd->_keyboardEventAction);
  }

  // --------------------
  // for eject key
  struct HookedConsumer {
    IOHIKeyboard *kbd;
    KeyboardSpecialEventCallback origSpecialEventCallback;
    // save SpecialEventTarget for keyToConsumer
    OSObject *origSpecialEventTarget;

    void initialize(IOHIKeyboard *_kbd);
    void refresh(void);
    void terminate(void);
  };
  static HookedConsumer hookedConsumer[MAXNUM_CONSUMER];
  static HookedConsumer *new_hookedConsumer(void);
  static HookedConsumer *search_hookedConsumer(const IOHIKeyboard *kbd);
  static HookedConsumer *get_1stHookedConsumer(void);
  static KeyboardSpecialEventCallback getKeyboardSpecialEventCallback(IOHIKeyboard *kbd) {
    return reinterpret_cast<KeyboardSpecialEventCallback>(kbd->_keyboardSpecialEventAction);
  }

  // --------------------
  struct HookedPointing {
    IOHIPointing *pointing;
    RelativePointerEventCallback origRelativePointerEventCallback;
    ScrollWheelEventCallback origScrollWheelEventCallback;
    // save EventTarget for keyToPointing.
    OSObject *relativePointerEventTarget;
    OSObject *scrollWheelEventTarget;

    void initialize(IOHIPointing *_pointing);
    void refresh(void);
    void terminate(void);
  };
  static HookedPointing hookedPointing[MAXNUM_POINTING];
  static HookedPointing *new_hookedPointing(void);
  static HookedPointing *search_hookedPointing(const IOHIPointing *pointing);
  static HookedPointing *get_1stHookedPointing(void);
  static RelativePointerEventCallback getRelativePointerEventCallback(IOHIPointing *pointing) {
    return reinterpret_cast<RelativePointerEventCallback>(pointing->_relativePointerEventAction);
  }
  static ScrollWheelEventCallback getScrollWheelEventCallback(IOHIPointing *pointing) {
    return reinterpret_cast<ScrollWheelEventCallback>(pointing->_scrollWheelEventAction);
  }

  // ------------------------------------------------------------
  static void refreshHookedDevice(OSObject *owner, IOTimerEventSource *sender);

  // ------------------------------------------------------------
  static bool notifierfunc_hookKeyboard(org_pqrs_driver_KeyRemap4MacBook *self, void *ref, IOService *newService);
  static bool notifierfunc_unhookKeyboard(org_pqrs_driver_KeyRemap4MacBook *self, void *ref, IOService *newService);

  static bool notifierfunc_hookPointing(org_pqrs_driver_KeyRemap4MacBook *self, void *ref, IOService *newService);
  static bool notifierfunc_unhookPointing(org_pqrs_driver_KeyRemap4MacBook *self, void *ref, IOService *newService);

  static bool replaceKeyboardEvent(org_pqrs_driver_KeyRemap4MacBook *self, IOHIKeyboard *kbd);
  static bool restoreKeyboardEvent(org_pqrs_driver_KeyRemap4MacBook *self, IOHIKeyboard *kbd);
  static bool replacePointingEvent(org_pqrs_driver_KeyRemap4MacBook *self, IOHIPointing *pointing);
  static bool restorePointingEvent(org_pqrs_driver_KeyRemap4MacBook *self, IOHIPointing *pointing);

  IONotifier *notifier_hookKeyboard;
  IONotifier *notifier_unhookKeyboard;

  IONotifier *notifier_hookPointing;
  IONotifier *notifier_unhookPointing;

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
  static void doExtraKeyRepeat(OSObject *owner, IOTimerEventSource *sender);

  // --------------------
  static void keyboardSpecialEventCallBack(OSObject *target,
                                           unsigned int eventType,
                                           unsigned int flags,
                                           unsigned int key,
                                           unsigned int flavor,
                                           UInt64 guid,
                                           bool repeat,
                                           AbsoluteTime ts,
                                           OSObject *sender,
                                           void *refcon);

  // --------------------
  static void relativePointerEventCallBack(OSObject *target,
                                           int buttons,
                                           int dx,
                                           int dy,
                                           AbsoluteTime ts,
                                           OSObject *sender,
                                           void *refcon);

  static void scrollWheelEventCallback(OSObject * target,
                                       short      deltaAxis1,
                                       short      deltaAxis2,
                                       short      deltaAxis3,
                                       IOFixed    fixedDelta1,
                                       IOFixed    fixedDelta2,
                                       IOFixed    fixedDelta3,
                                       SInt32     pointDelta1,
                                       SInt32     pointDelta2,
                                       SInt32     pointDelta3,
                                       SInt32     options,
                                       AbsoluteTime ts,
                                       OSObject * sender,
                                       void *     refcon);
};

#endif

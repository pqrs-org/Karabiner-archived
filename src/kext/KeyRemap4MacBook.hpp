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
    MAXNUM_POINTING = 4,
  };

  IOWorkLoop *workLoop;
  IOTimerEventSource *refreshTimer;

  // ------------------------------------------------------------
  struct HookedKeyboard {
    IOHIKeyboard *kbd;
    KeyboardEventCallback origEventCallback;

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
      OSObject *sender;
      void *refcon;
    } repeat;

    void initialize(IOHIKeyboard *_kbd, IOWorkLoop *workLoop);
    void refresh(void);
    void terminate(IOWorkLoop *workLoop);
  };
  static HookedKeyboard hookedKeyboard[MAXNUM_KEYBOARD];
  static HookedKeyboard *new_hookedKeyboard(void);
  static HookedKeyboard *search_hookedKeyboard(const IOHIKeyboard *kbd);
  static KeyboardEventCallback getKeyboardEventCallback(IOHIKeyboard *kbd) {
    return reinterpret_cast<KeyboardEventCallback>(kbd->_keyboardEventAction);
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
  static bool replacePointingEvent(org_pqrs_driver_KeyRemap4MacBook *self, IOHIPointing *pointing);

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

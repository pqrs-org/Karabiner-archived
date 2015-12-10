#ifndef LISTHOOKEDDEVICE_HPP
#define LISTHOOKEDDEVICE_HPP

#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/hidsystem/IOHIDevice.h>
END_IOKIT_INCLUDE;

#include "KeyCode.hpp"
#include "List.hpp"
#include "PressingPhysicalKeys.hpp"
#include "WeakPointer.hpp"

namespace org_pqrs_Karabiner {
class TimerWrapper;

class ListHookedDevice {
public:
  DECLARE_WEAKPOINTER(Item);

  class Item : public List::Item {
    friend class ListHookedDevice;

  protected:
    Item(IOHIDevice* d) : device_(d),
                          deviceType_(DeviceType::UNKNOWN),
                          inProgress_(false) {
      setDeviceIdentifier();
      setDeviceType();

      WeakPointerManager_Item::add(this);
    }

    virtual ~Item(void) {
      WeakPointerManager_Item::remove(this);
    };

  public:
    virtual bool isReplaced(void) const = 0;

    const IOHIDevice* get(void) const { return device_; }
    const DeviceIdentifier& getDeviceIdentifier(void) const { return deviceIdentifier_; }
    DeviceType::DeviceType getDeviceType(void) const { return deviceType_; }

    bool isInternalDevice(void) const { return deviceType_ == DeviceType::APPLE_INTERNAL; }

    void updatePressingPhysicalKeys(const Params_Base& paramsBase) {
      pressingPhysicalKeys_.update(paramsBase);
    }

  protected:
    IOHIDevice* device_;
    DeviceIdentifier deviceIdentifier_;
    DeviceType::DeviceType deviceType_;

    // When immediately after a device has been connected,
    // EventAction (eg. kbd->_keyboardEventAction) might be null.
    // In that case, EventAction will be updated after few seconds.
    //
    // We mark such device by inProgress_ flag and run timer until inProgress_ is true.
    bool inProgress_;

    PressingPhysicalKeys pressingPhysicalKeys_;

    virtual bool refresh(void) = 0;

    void setDeviceIdentifier(void);
    void setDeviceType(void);
    static bool isConsumer(const char* name);
  };

  bool initialize(void);
  void terminate(void);

  void push_back(ListHookedDevice::Item* newp);
  void erase(IOHIDevice* p);

  void refresh(void);
  bool isInProgress(void) const;

  size_t pressingPhysicalKeysCount(void) const;
  void clearPressingPhysicalKeysCount(void) const;

  ListHookedDevice::Item* get(const IOHIDevice* device);
  ListHookedDevice::Item* get_replaced(void);

  void getDeviceInformation(BridgeDeviceInformation& out, size_t index) const;

  static void initializeAll(IOWorkLoop& workloop);
  static void terminateAll(void);
  static void refreshAll(void);
  static size_t pressingPhysicalKeysCountAll(void);
  static void clearInternalKeyboardPressingPhysicalKeysCountAll(void);

  static void start_refreshInProgressDevices_timer(void);
  static void refreshInProgressDevices_timer_callback(OSObject* owner, IOTimerEventSource* sender);

protected:
  ListHookedDevice(void) : last_(nullptr) {}
  virtual ~ListHookedDevice(void) {}

  const IOHIDevice* last_;
  List list_;

private:
  enum {
    REFRESH_INPROGRESS_DEVICES_TIMER_INTERVAL = 1000,
  };
  static TimerWrapper refreshInProgressDevices_timer_;
};
}

#endif

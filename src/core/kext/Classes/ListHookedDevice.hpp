#ifndef LISTHOOKEDDEVICE_HPP
#define LISTHOOKEDDEVICE_HPP

#include <IOKit/hidsystem/IOHIDevice.h>

#include "KeyCode.hpp"
#include "List.hpp"

namespace org_pqrs_Karabiner {
class TimerWrapper;

class ListHookedDevice {
public:
  class Item : public List::Item {
    friend class ListHookedDevice;

  protected:
    Item(IOHIDevice* d);
    virtual ~Item(void){};

  public:
    virtual bool isReplaced(void) const = 0;

    const IOHIDevice* get(void) const { return device_; }
    const DeviceIdentifier& getDeviceIdentifier(void) const { return deviceIdentifier_; }
    DeviceType::DeviceType getDeviceType(void) const { return deviceType_; }

    bool isInternalDevice(void) const { return deviceType_ == DeviceType::APPLE_INTERNAL; }

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

  ListHookedDevice::Item* get(const IOHIDevice* device);
  ListHookedDevice::Item* get_replaced(void);

  void getDeviceInformation(BridgeDeviceInformation& out, size_t index) const;

  static void initializeAll(IOWorkLoop& workloop);
  static void terminateAll(void);
  static void refreshAll(void);

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

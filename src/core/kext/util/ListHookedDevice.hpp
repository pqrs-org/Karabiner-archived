#ifndef LISTHOOKEDDEVICE_HPP
#define LISTHOOKEDDEVICE_HPP

#include "base.hpp"
#include "KeyCode.hpp"
#include "List.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ListHookedDevice {
  public:
    class Item : public List::Item {
      friend class ListHookedDevice;

    public:
      Item(IOHIDevice* d);
      virtual ~Item(void) {};

      IOHIDevice* get(void) const { return device_; }
      bool isEqualVendorIDProductID(DeviceVendorID vendorID, DeviceProductID productID) const;
      DeviceVendorID getVendorID(void) const { return vendorID_; }
      DeviceProductID getProductID(void) const { return productID_; }

    protected:
      IOHIDevice* device_;
      DeviceVendorID vendorID_;
      DeviceProductID productID_;
      DeviceType::DeviceType deviceType_;

      virtual bool refresh_callback(void) = 0;
      virtual bool isReplaced(void) const = 0;

      void setVendorIDProductID(void);
      void setDeviceType(void);
      static bool isConsumer(const char* name);
    };

    bool initialize(void);
    void terminate(void);

    void push_back(ListHookedDevice::Item* newp);
    void erase(IOHIDevice* p);

    ListHookedDevice::Item* get(const IOHIDevice* device);
    ListHookedDevice::Item* get(void);

    void refresh_callback(void);

  protected:
    ListHookedDevice(void) : last_(NULL), list_(NULL), lock_(NULL) {}
    virtual ~ListHookedDevice(void) {}

  private:
    ListHookedDevice::Item* get_nolock(const IOHIDevice* device);

    const IOHIDevice* last_;
    List* list_;
    IOLock* lock_;
  };
}

#endif

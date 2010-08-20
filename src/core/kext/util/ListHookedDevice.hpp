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

      virtual bool isReplaced(void) const = 0;

      IOHIDevice* get(void) const { return device_; }
      bool isEqualVendorIDProductID(DeviceVendorID vendorID, DeviceProductID productID) const;
      bool isEqualVendorID(DeviceVendorID vendorID) const;
      DeviceVendorID getVendorID(void) const { return vendorID_; }
      DeviceProductID getProductID(void) const { return productID_; }

    protected:
      IOHIDevice* device_;
      DeviceVendorID vendorID_;
      DeviceProductID productID_;
      DeviceType::DeviceType deviceType_;

      virtual bool refresh_callback(void) = 0;

      void setVendorIDProductID(void);
      void setDeviceType(void);
      static bool isConsumer(const char* name);
    };

    bool initialize(void);
    void terminate(void);

    void push_back(ListHookedDevice::Item* newp);
    void erase(IOHIDevice* p);

    void refresh_callback(void);

    // We need to call get_nolock with "ScopedLock lk(getListLock())".
    ListHookedDevice::Item* get_nolock(const IOHIDevice* device);
    ListHookedDevice::Item* get_replaced_nolock(void);
    IOLock* getListLock(void) { return list_lock_; }

  protected:
    ListHookedDevice(void) : last_(NULL), list_(NULL), list_lock_(NULL) {}
    virtual ~ListHookedDevice(void) {}

    const IOHIDevice* last_;
    List* list_;
    IOLock* list_lock_;
  };
}

#endif

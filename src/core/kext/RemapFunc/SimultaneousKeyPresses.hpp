#ifndef SIMULTANEOUSKEYPRESSES_HPP
#define SIMULTANEOUSKEYPRESSES_HPP

#include "EventInputQueue.hpp"
#include "KeyToKey.hpp"
#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class SimultaneousKeyPresses final : public RemapFuncBase {
public:
  SimultaneousKeyPresses(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_SIMULTANEOUSKEYPRESSES, autogenId),
                                                isUseSeparator_(false),
                                                index_(0),
                                                isFromInfoFull_(false),
                                                isToRaw_(false),
                                                isPostFromEventsAsRaw_(false),
                                                isStrictKeyOrder_(false),
                                                toKey_raw_(KeyCode::VK_NONE),
                                                keytokey_(autogenId) {}

  // This function changes Simultaneous key presses to KeyCode::VK_SIMULTANEOUSKEYPRESSES_xxx
  RemapSimultaneousKeyPressesResult::Value remapSimultaneousKeyPresses(void) override;
  // This function changes KeyCode::VK_SIMULTANEOUSKEYPRESSES_xxx to remapped key/pointing events.
  bool remap(RemapParams& remapParams) override;

  // ----------------------------------------
  // [0] => virtualkey
  // [1] => fromKey1_
  // [2] => fromKey2_
  // [3] => toKeys_[0]
  // [4] => toKeys_[1]
  // [5] => ...
  void add(AddDataType datatype, AddValue newval) override;

  static void clearActiveFromInfos(void) {
    ActiveFromInfos::clear();
  }

private:
  void push_remapped(bool isKeyDown, const DeviceIdentifier& deviceIdentifier, const ListHookedDevice::WeakPointer_Item& device);

  class FromInfo;

  class ActiveFromInfos final {
    friend class FromInfo;

  public:
    class Item final : public List::Item {
    public:
      Item(const FromInfo* p, const ListHookedDevice::WeakPointer_Item& d) : fromInfo_(p), device_(d) {}
      ~Item(void) {}

      const FromInfo* getFromInfo(void) const { return fromInfo_; }
      const ListHookedDevice::WeakPointer_Item& getDevice(void) const { return device_; }

    private:
      const FromInfo* fromInfo_;
      const ListHookedDevice::WeakPointer_Item device_;
    };

    static Item* find(const FromInfo* p, const ListHookedDevice::WeakPointer_Item& d) {
      for (Item* q = static_cast<Item*>(list_.safe_front()); q; q = static_cast<Item*>(q->getnext())) {
        if (q->getFromInfo() == p && q->getDevice() == d) {
          return q;
        }
      }
      return nullptr;
    }

    static void clear(void) {
      list_.clear();
    }

    static void push_back(const FromInfo* p, const ListHookedDevice::WeakPointer_Item& d) {
      erase_expired();

      list_.push_back(new Item(p, d));
    }

    static bool erase(const FromInfo* p, const ListHookedDevice::WeakPointer_Item& d) {
      erase_expired();

      auto q = find(p, d);
      if (q) {
        list_.erase_and_delete(q);
        return true;
      }
      return false;
    }

    static void erase_all(const FromInfo* p) {
      erase_expired();

      Item* q = static_cast<Item*>(list_.safe_front());
      while (q) {
        if (q->getFromInfo() == p) {
          q = static_cast<Item*>(list_.erase_and_delete(q));
        } else {
          q = static_cast<Item*>(q->getnext());
        }
      }
    }

  private:
    static void erase_expired(void) {
      Item* q = static_cast<Item*>(list_.safe_front());
      while (q) {
        if ((q->getDevice()).expired()) {
          q = static_cast<Item*>(list_.erase_and_delete(q));
        } else {
          q = static_cast<Item*>(q->getnext());
        }
      }
    }

    static List list_;
  };

  class FromInfo final {
  public:
    FromInfo(void) : fromEvent_() {}
    FromInfo(AddDataType datatype, AddValue v) : fromEvent_(datatype, v) {}

    ~FromInfo(void) {
      ActiveFromInfos::erase_all(this);
    }

    void activate(const ListHookedDevice::WeakPointer_Item& d) {
      ActiveFromInfos::push_back(this, d);
    }
    void deactivate(ListHookedDevice::WeakPointer_Item& d) {
      ActiveFromInfos::erase(this, d);
    }

    bool isActive(ListHookedDevice::WeakPointer_Item& d) const { return ActiveFromInfos::find(this, d); }

    const FromEvent& fromEvent(void) { return fromEvent_; }

  private:
    FromEvent fromEvent_;
  };
  DECLARE_VECTOR(FromInfo);

  class DownKeys final {
  public:
    DownKeys(void) : item(nullptr) {}
    DownKeys(EventInputQueue::Item* i) : item(i) {}
    EventInputQueue::Item* item;
  };
  DECLARE_VECTOR(DownKeys);

  // --------------------
  bool isUseSeparator_;
  size_t index_;
  KeyCode virtualkey_;

  Vector_FromInfo fromInfo_;
  bool isFromInfoFull_;
  Vector_ModifierFlag fromModifierFlags_;

  // --------------------
  bool isToRaw_;
  bool isPostFromEventsAsRaw_;
  bool isStrictKeyOrder_;
  KeyCode toKey_raw_;

  KeyToKey keytokey_;

  Vector_DownKeys downKeys_;
};
}
}

#endif

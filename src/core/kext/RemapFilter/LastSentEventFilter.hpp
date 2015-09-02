#ifndef LASTSENTEVENTFILTER_HPP
#define LASTSENTEVENTFILTER_HPP

#include "CommonData.hpp"
#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFilter {
class LastSentEventFilter final : public RemapFilterBase {
public:
  LastSentEventFilter(unsigned int type, const unsigned int* vec, size_t length) : RemapFilterBase(type) {
    targets_.reserve(length / 2);

    int i = 0;
    while (i < static_cast<int>(length) - 1) {
      targets_.push_back(Item(AddDataType(vec[i]), AddValue(vec[i + 1])));
      i += 2;

      // set EventType
      if (i < static_cast<int>(length) - 1) {
        if (vec[i] == BRIDGE_DATATYPE_EVENTTYPE) {
          targets_.back().eventType = EventType(vec[i + 1]);
          i += 2;
          continue;
        }
      }
    }

    if (length % 2 > 0) {
      IOLOG_WARN("Invalid length(%d) in BRIDGE_FILTERTYPE_LASTSENTEVENT_*\n", static_cast<int>(length));
    }
  }

  bool
  isblocked(void) override {
    if (get_type() == BRIDGE_FILTERTYPE_LASTSENTEVENT_NOT ||
        get_type() == BRIDGE_FILTERTYPE_LASTSENTEVENT_ONLY) {

      auto& current = CommonData::getcurrent_lastsentevent();
      if (current.get_datatype() == BRIDGE_DATATYPE_NONE) return false;

      bool isnot = (get_type() == BRIDGE_FILTERTYPE_LASTSENTEVENT_NOT);

      for (size_t i = 0; i < targets_.size(); ++i) {
        if (targets_[i].dataType == current.get_datatype() &&
            targets_[i].value == current.get_value() &&
            (targets_[i].eventType == current.get_eventtype() || targets_[i].eventType == EventType::NONE)) {
          return isnot ? true : false;
        }
      }

      return isnot ? false : true;
    }

    IOLOG_ERROR("LastSentEventFilter::isblocked unknown type(%d)\n", get_type());
    return false;
  }

private:
  class Item final {
  public:
    Item(void) : eventType(EventType::NONE) {}
    Item(AddDataType d, AddValue v) : dataType(d), value(v), eventType(EventType::NONE) {}
    AddDataType dataType;
    AddValue value;
    EventType eventType;
  };
  DECLARE_VECTOR(Item);

  Vector_Item targets_;
};
}
}

#endif

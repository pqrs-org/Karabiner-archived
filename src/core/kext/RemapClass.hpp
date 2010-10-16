#ifndef REMAPCLASS_HPP
#define REMAPCLASS_HPP

#include "bridge.h"
#include "remap.hpp"
#include "RemapFunc/KeyToKey.hpp"
#include "RemapFunc/KeyToConsumer.hpp"
#include "RemapFunc/KeyToPointingButton.hpp"
#include "RemapFunc/ConsumerToConsumer.hpp"
#include "RemapFunc/ConsumerToKey.hpp"
#include "RemapFunc/DoublePressModifier.hpp"
#include "RemapFunc/DropKeyAfterRemap.hpp"
#include "RemapFunc/HoldingKeyToKey.hpp"
#include "RemapFunc/IgnoreMultipleSameKeyPress.hpp"
#include "RemapFunc/KeyOverlaidModifier.hpp"
#include "RemapFunc/PointingButtonToKey.hpp"
#include "RemapFunc/PointingButtonToPointingButton.hpp"
#include "RemapFunc/PointingRelativeToScroll.hpp"
#include "RemapFunc/SimultaneousKeyPresses.hpp"
#include "RemapFilter/FilterUnion.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class RemapClass {
  public:
    class Item {
    public:
      Item(void) {
        type_ = BRIDGE_REMAPTYPE_NONE;
        filters_ = NULL;
      }
      void initialize_remap(const unsigned int* vec, size_t length);
      void initialize_filter(const unsigned int* vec, size_t length);
      void terminate(void);

      // --------------------
      bool remap(RemapParams& remapParams);
      bool remap(RemapConsumerParams& remapParams);
      bool remap(RemapPointingParams_relative& remapParams);
      // for DropKeyAfterRemap
      bool drop(const Params_KeyboardEventCallBack& params);
      //
      void remap_SimultaneousKeyPresses(void);

    private:
      bool isblocked(void);

      unsigned int type_;

      union {
        RemapFunc::KeyToKey* keyToKey;
        RemapFunc::KeyToConsumer* keyToConsumer;
        RemapFunc::KeyToPointingButton* keyToPointingButton;
        RemapFunc::ConsumerToConsumer* consumerToConsumer;
        RemapFunc::ConsumerToKey* consumerToKey;
        RemapFunc::DoublePressModifier* doublePressModifier;
        RemapFunc::DropKeyAfterRemap* dropKeyAfterRemap;
        RemapFunc::HoldingKeyToKey* holdingKeyToKey;
        RemapFunc::IgnoreMultipleSameKeyPress* ignoreMultipleSameKeyPress;
        RemapFunc::KeyOverlaidModifier* keyOverlaidModifier;
        RemapFunc::PointingButtonToKey* pointingButtonToKey;
        RemapFunc::PointingButtonToPointingButton* pointingButtonToPointingButton;
        RemapFunc::PointingRelativeToScroll* pointingRelativeToScroll;
        RemapFunc::SimultaneousKeyPresses* simultaneousKeyPresses;
      } p_;

      RemapFilter::Vector_FilterUnionPointer* filters_;
    };
    DECLARE_VECTOR(Item);

    // ----------------------------------------------------------------------
    RemapClass(const unsigned int* vec, size_t length,
               const unsigned int* filter, size_t filterlength,
               bool enable_when_passthrough);
    ~RemapClass(void);

    void remap_key(RemapParams& remapParams);
    void remap_consumer(RemapConsumerParams& remapParams);
    void remap_pointing(RemapPointingParams_relative& remapParams);
    void remap_simultaneouskeypresses(void);
    bool remap_dropkeyafterremap(const Params_KeyboardEventCallBack& params);
    bool enabled(void);

  private:
    Vector_Item items_;
  };

  // ================================================================================
  namespace RemapClassManager {
    void initialize(IOWorkLoop& workloop);
    void terminate(void);
    void refresh(void);

    void remap_setkeyboardtype(KeyboardType& keyboardType);
    void remap_key(RemapParams& remapParams);
    void remap_consumer(RemapConsumerParams& remapParams);
    void remap_pointing(RemapPointingParams_relative& remapParams);
    void remap_simultaneouskeypresses(void);

    // return true if dropped.
    bool remap_dropkeyafterremap(const Params_KeyboardEventCallBack& params);

    bool isEventInputQueueDelayEnabled(void);
  };
}

#endif

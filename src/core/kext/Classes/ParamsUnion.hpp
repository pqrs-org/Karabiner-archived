#ifndef PARAMSUNION_HPP
#define PARAMSUNION_HPP

#include "CallBackWrapper.hpp"

namespace org_pqrs_Karabiner {
  class ParamsUnion {
  public:
    explicit ParamsUnion(const Params_KeyboardEventCallBack& p)
    {
      params_ = Params_KeyboardEventCallBack::alloc(p);
    }
    explicit ParamsUnion(const Params_UpdateEventFlagsCallback& p)
    {
      params_ = Params_UpdateEventFlagsCallback::alloc(p);
    }
    explicit ParamsUnion(const Params_KeyboardSpecialEventCallback& p)
    {
      params_ = Params_KeyboardSpecialEventCallback::alloc(p);
    }
    explicit ParamsUnion(const Params_RelativePointerEventCallback& p)
    {
      params_ = Params_RelativePointerEventCallback::alloc(p);
    }
    explicit ParamsUnion(const Params_ScrollWheelEventCallback& p)
    {
      params_ = Params_ScrollWheelEventCallback::alloc(p);
    }
    explicit ParamsUnion(const Params_Wait& p)
    {
      params_ = Params_Wait::alloc(p);
    }
    explicit ParamsUnion(const ParamsUnion& rhs)
    {
      params_ = (rhs.params_ != NULL) ? Params_Factory::copy(*(rhs.params_)) : NULL;
    }

    ~ParamsUnion(void)
    {
      if (params_) {
        delete params_;
      }
    }

    const Params_Base& get_Params_Base(void) const { return Params_Base::safe_dereference(params_); }

    const Params_KeyboardEventCallBack* get_Params_KeyboardEventCallBack(void) const
    {
      if (! params_) return NULL;
      return params_->get_Params_KeyboardEventCallBack();
    }
    const Params_UpdateEventFlagsCallback* get_Params_UpdateEventFlagsCallback(void) const
    {
      if (! params_) return NULL;
      return params_->get_Params_UpdateEventFlagsCallback();
    }
    const Params_KeyboardSpecialEventCallback* get_Params_KeyboardSpecialEventCallback(void) const
    {
      if (! params_) return NULL;
      return params_->get_Params_KeyboardSpecialEventCallback();
    }
    const Params_RelativePointerEventCallback* get_Params_RelativePointerEventCallback(void) const
    {
      if (! params_) return NULL;
      return params_->get_Params_RelativePointerEventCallback();
    }
    const Params_ScrollWheelEventCallback* get_Params_ScrollWheelEventCallback(void) const
    {
      if (! params_) return NULL;
      return params_->get_Params_ScrollWheelEventCallback();
    }
    const Params_Wait* get_Params_Wait(void) const
    {
      if (! params_) return NULL;
      return params_->get_Params_Wait();
    }

    bool iskeydown(bool& output) const
    {
      if (! params_) return false;
      return params_->iskeydown(output);
    }

    bool isModifier(void) const
    {
      if (! params_) return false;
      return params_->isModifier();
    }

  private:
    ParamsUnion& operator=(const ParamsUnion& rhs); // Prevent assignment

    Params_Base* params_;
  };
}

#endif

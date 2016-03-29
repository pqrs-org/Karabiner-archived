#ifndef SHELLCOMMANDFILTER_HPP
#define SHELLCOMMANDFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFilter {
class ShellCommandFilter final : public RemapFilterBase {
public:
    ShellCommandFilter(unsigned int type, const unsigned int* vec, size_t length) : RemapFilterBase(type) {
        //targets_.reserve(length);

        for (size_t i = 0; i < length; ++i) {
            //targets_.push_back(WorkspaceShellCommandId(vec[i]));
        }
    }

    bool
    isblocked(void) override {
        return true;
        
        /*char cmd[] = "";
        char mute[] = " >/dev/null";
        int ret = system(mute);
        
        if (ret == EXIT_SUCCESS) {
            std::cout << "success" << std::endl;;
        } else {
            std::cout << "failure" << std::endl;;
        }*/
        
        
        /*if (get_type() == BRIDGE_FILTERTYPE_WINDOWNAME_NOT ||
            get_type() == BRIDGE_FILTERTYPE_WINDOWNAME_ONLY) {

            bool isnot = (get_type() == BRIDGE_FILTERTYPE_WINDOWNAME_NOT);

            for (size_t i = 0; i < targets_.size(); ++i) {
                if (CommonData::getcurrent_workspaceWindowNameIds().is_include(targets_[i])) {
                    return isnot ? true : false;
                }
            }

            return isnot ? false : true;
        }

        IOLOG_ERROR("ShellCommandFilter::isblocked unknown type(%d)\n", get_type());
        return false;*/
    }

private:
    //Vector_WorkspaceShellCommandId targets_;
};
}
}

#endif

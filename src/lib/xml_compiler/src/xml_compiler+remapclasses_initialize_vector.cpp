#include "pqrs/bridge.h"
#include "pqrs/vector.hpp"
#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  xml_compiler::remapclasses_initialize_vector::remapclasses_initialize_vector(void)
  {
    clear();
  }

  void
  xml_compiler::remapclasses_initialize_vector::clear(void)
  {
    data_.resize(2);
    data_[INDEX_OF_FORMAT_VERSION] = BRIDGE_REMAPCLASS_INITIALIZE_VECTOR_FORMAT_VERSION;
    data_[INDEX_OF_COUNT] = 0;

    is_configindex_added_.clear();

    max_configindex_ = 0;
    freezed_ = false;
  }

  const std::vector<uint32_t>&
  xml_compiler::remapclasses_initialize_vector::get(void) const
  {
    if (! freezed_) {
      throw xml_compiler_logic_error("remapclasses_initialize_vector is not freezed.");
    }
    return data_;
  }

  void
  xml_compiler::remapclasses_initialize_vector::add(const std::vector<uint32_t>& v,
                                                    uint32_t configindex,
                                                    const std::string& raw_identifier)
  {
    if (freezed_) {
      throw xml_compiler_logic_error("remapclasses_initialize_vector is freezed.");
    }
    if (is_configindex_added_.find(configindex) != is_configindex_added_.end()) {
      throw xml_compiler_runtime_error(boost::format("Duplicated identifier:\n"
                                                     "\n"
                                                     "<identifier>%1%</identifier>") %
                                       raw_identifier);
    }

    // size
    data_.push_back(v.size() + 1); // +1 == configindex
    // configindex
    data_.push_back(configindex);
    // data
    pqrs::vector::push_back(data_, v);

    ++(data_[INDEX_OF_COUNT]);

    if (configindex > max_configindex_) {
      max_configindex_ = configindex;
    }
    is_configindex_added_[configindex] = true;
  }

  void
  xml_compiler::remapclasses_initialize_vector::freeze(void)
  {
    if (freezed_) {
      throw xml_compiler_logic_error("remapclasses_initialize_vector is already freezed.");
    }

    for (uint32_t i = 0; i < max_configindex_; ++i) {
      if (is_configindex_added_.find(i) == is_configindex_added_.end()) {
        std::vector<uint32_t> v;
        add(v, i, "");
      }
    }

    freezed_ = true;
  }
}

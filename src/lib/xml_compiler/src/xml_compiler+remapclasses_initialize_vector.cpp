#include "bridge.h"
#include "pqrs/vector.hpp"
#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  xml_compiler::remapclasses_initialize_vector::remapclasses_initialize_vector(void)
  {
    clear();
    freeze();
  }

  void
  xml_compiler::remapclasses_initialize_vector::clear(void)
  {
    data_.resize(2);
    data_[INDEX_OF_FORMAT_VERSION] = BRIDGE_REMAPCLASS_INITIALIZE_VECTOR_FORMAT_VERSION;
    data_[INDEX_OF_CONFIG_COUNT] = 0;

    is_config_index_added_.clear();

    max_config_index_ = 0;
    freezed_ = false;
    start_index_ = 0;
    ended_ = true;
  }

  const std::vector<uint32_t>&
  xml_compiler::remapclasses_initialize_vector::get(void) const
  {
    assert(freezed_);
    return data_;
  }

  uint32_t
  xml_compiler::remapclasses_initialize_vector::get_config_count(void) const
  {
    assert(freezed_);
    return data_[INDEX_OF_CONFIG_COUNT];
  }

  void
  xml_compiler::remapclasses_initialize_vector::freeze(void)
  {
    assert(! freezed_);

    cleanup_();

    for (uint32_t i = 0; i < max_config_index_; ++i) {
      if (is_config_index_added_.find(i) == is_config_index_added_.end()) {
        start(i, "");
        end();
      }
    }

    freezed_ = true;
  }

  void
  xml_compiler::remapclasses_initialize_vector::start(uint32_t config_index,
                                                      const std::string& raw_identifier)
  {
    assert( ! freezed_);

    if (is_config_index_added_.find(config_index) != is_config_index_added_.end()) {
      throw xml_compiler_runtime_error(boost::format("Duplicated identifier:\n"
                                                     "\n"
                                                     "<identifier>%1%</identifier>") %
                                       raw_identifier);
    }

    if (! ended_) {
      cleanup_();
    }

    start_index_ = data_.size();
    ended_ = false;

    // size
    data_.push_back(1); // 1 == count of config_index
    // config_index
    data_.push_back(config_index);
  }

  void
  xml_compiler::remapclasses_initialize_vector::end(void)
  {
    ended_ = true;

    // ----------------------------------------
    ++(data_[INDEX_OF_CONFIG_COUNT]);

    // ----------------------------------------
    uint32_t config_index = data_[start_index_ + 1];
    if (config_index > max_config_index_) {
      max_config_index_ = config_index;
    }
    is_config_index_added_[config_index] = true;
  }

  void
  xml_compiler::remapclasses_initialize_vector::cleanup_(void)
  {
    if (ended_) return;

    data_.resize(start_index_);
    ended_ = true;
  }
}

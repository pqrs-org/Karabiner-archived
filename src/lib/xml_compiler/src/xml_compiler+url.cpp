#include <exception>
#include "pqrs/xml_compiler.hpp"
#include "bridge.h"

namespace pqrs {
  xml_compiler::url_loader::~url_loader(void)
  {
    std::string raw_identifier("system.vk_open_url_definition");
    auto identifier = raw_identifier;
    normalize_identifier_(identifier);

    uint32_t config_index = symbol_map_.add("ConfigIndex", identifier);
    identifier_map_[config_index] = raw_identifier;

    try {
      remapclasses_initialize_vector_.start(config_index, raw_identifier);
      {
        for (const auto& it : vk_open_url_map_) {
          remapclasses_initialize_vector_.push_back(2);
          remapclasses_initialize_vector_.push_back(BRIDGE_VK_OPEN_URL);
          remapclasses_initialize_vector_.push_back(it.first);
        }
      }
      remapclasses_initialize_vector_.end();
    } catch (std::exception& e) {
      assert(! "exception in ~url_loader");
    }
  }

  void
  xml_compiler::url_loader::traverse(const extracted_ptree& pt) const
  {
    for (const auto& it : pt) {
      if (it.get_tag_name() != "vkopenurldef") {
        if (! it.children_empty()) {
          traverse(it.children_extracted_ptree());
        }
      } else {
        std::tr1::shared_ptr<url> newurl(new url());
        if (! newurl) continue;

        // ----------------------------------------
        bool error = false;

        for (const auto& child : it.children_extracted_ptree()) {
          if (child.get_tag_name() == "name") {
            newurl->set_name(pqrs::string::remove_whitespaces_copy(child.get_data()));

            if (! boost::starts_with(*(newurl->get_name()), "KeyCode::VK_OPEN_URL_")) {
              error = true;
              xml_compiler_.error_information_.set(boost::format("<name> within <vkopenurldef> must start with \"KeyCode::VK_OPEN_URL_\":\n\n<name>%1%</name>") %
                                                   *(newurl->get_name()));
            }

          } else if (child.get_tag_name() == "url") {
            newurl->set_url(boost::trim_copy(child.get_data()));

            auto type = child.get_optional("<xmlattr>.type");
            if (type) {
              newurl->set_type(boost::trim_copy(*type));
            }
          }
        }

        if (error) {
          continue;
        }

        // ----------------------------------------
        // Validation

        // name
        if (! newurl->get_name()) {
          xml_compiler_.error_information_.set(boost::format("No <name> within <%1%>.") %
                                               it.get_tag_name());
          continue;
        }

        if (newurl->get_name()->empty()) {
          xml_compiler_.error_information_.set(boost::format("Empty <name> within <%1%>.") %
                                               it.get_tag_name());
          continue;
        }

        // url
        if (! newurl->get_url()) {
          xml_compiler_.error_information_.set(boost::format("No <url> within <%1%>.") %
                                               it.get_tag_name());
          continue;
        }

        if (newurl->get_url()->empty()) {
          xml_compiler_.error_information_.set(boost::format("Empty <url> within <%1%>.") %
                                               it.get_tag_name());
          continue;
        }

        // ----------------------------------------
        // register to symbol_map_.
        if (! symbol_map_.get_optional(*(newurl->get_name()))) {
          auto keycode = symbol_map_.add("KeyCode",
                                         boost::replace_first_copy(*(newurl->get_name()), "KeyCode::", ""));
          vk_open_url_map_[keycode] = newurl;
        }
      }
    }
  }
}

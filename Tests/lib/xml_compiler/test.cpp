#define CATCH_CONFIG_MAIN
#include "../../include/catch.hpp"

#include <boost/property_tree/xml_parser.hpp>

#include "bridge.h"
#include "pqrs/xml_compiler.hpp"
#include "pqrs/xml_compiler_bindings_clang.h"

TEST_CASE("reload", "[pqrs_xml_compiler]") {
  pqrs::xml_compiler xml_compiler("data/system_xml", "data/private_xml");
  xml_compiler.reload();
  REQUIRE(xml_compiler.get_error_information().get_count() == 0);
  REQUIRE(xml_compiler.get_error_information().get_message() == "");

  {
    int v = 0;
    int space_is_ignored = 0;

    REQUIRE(xml_compiler.get_symbol_map().get_optional("ConfigIndex::system_vk_modifier_definition") == boost::optional<uint32_t>(v++));
    REQUIRE(xml_compiler.get_symbol_map().get_optional("ConfigIndex::system_vk_change_inputsource_definition") == boost::optional<uint32_t>(v++));
    REQUIRE(xml_compiler.get_symbol_map().get_optional("ConfigIndex::system_vk_open_url_definition") == boost::optional<uint32_t>(v++));
    REQUIRE(xml_compiler.get_symbol_map().get_optional("ConfigIndex::notsave_private_sample") == boost::optional<uint32_t>(v++));
    REQUIRE(xml_compiler.get_symbol_map().get_optional("ConfigIndex::notsave_passthrough") == boost::optional<uint32_t>(v++));
    REQUIRE(xml_compiler.get_symbol_map().get_optional("ConfigIndex::notsave_remap_sample") == boost::optional<uint32_t>(v++));
    REQUIRE(xml_compiler.get_symbol_map().get_optional("ConfigIndex::private_include_test") == boost::optional<uint32_t>(v++));
    REQUIRE(xml_compiler.get_symbol_map().get_optional("ConfigIndex::private_style_test_important") == boost::optional<uint32_t>(v++));
    REQUIRE(xml_compiler.get_symbol_map().get_optional("ConfigIndex::private_style_test_caution") == boost::optional<uint32_t>(v++));
    REQUIRE(xml_compiler.get_symbol_map().get_optional("ConfigIndex::private_replacement") == boost::optional<uint32_t>(v++));
    space_is_ignored = v;
    REQUIRE(xml_compiler.get_symbol_map().get_optional("ConfigIndex::private_space_is_ignored") == boost::optional<uint32_t>(v++));

    std::string expected = "private.space_ is_ ignored";
    REQUIRE(xml_compiler.get_identifier(space_is_ignored) == boost::optional<const std::string&>(expected));
    REQUIRE(xml_compiler.get_config_index(expected) == boost::optional<int>(space_is_ignored));
  }

  REQUIRE(xml_compiler.get_symbol_map().get_optional("ModifierFlag::CAPSLOCK") == boost::optional<uint32_t>(1));
  REQUIRE(xml_compiler.get_symbol_map().get_optional("ModifierFlag::SHIFT_L") == boost::optional<uint32_t>(2));
  REQUIRE(xml_compiler.get_symbol_map().get_optional("ModifierFlag::TEST1") == boost::optional<uint32_t>(13));
  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_MODIFIER_TEST1") != boost::none);
  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_MODIFIER_SHIFT_L") == boost::none);

  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::MY_INCLUDE_TEST_123") == boost::optional<uint32_t>(123));
  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::MY_INCLUDE_TEST_456") == boost::optional<uint32_t>(456));
  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::MY_INCLUDE_TEST_654") == boost::optional<uint32_t>(654));
  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::MY_INCLUDE_TEST_PARENT1") == boost::optional<uint32_t>(1));
  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::MY_INCLUDE_TEST_PARENT2") == boost::optional<uint32_t>(2));
  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::MY_INCLUDE_TEST_PARENT3") == boost::optional<uint32_t>(3));
  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::MY_LANG_KEY") == boost::optional<uint32_t>(123));
  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::SPACE_IS_IGNORED") == boost::optional<uint32_t>(999));

  // ------------------------------------------------------------
  uint32_t vk_change_inputsource_base = 1241;

  // JAPANESE
  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_CHANGE_INPUTSOURCE_JAPANESE") == boost::optional<uint32_t>(vk_change_inputsource_base));
  REQUIRE(xml_compiler.is_vk_change_inputsource_matched(vk_change_inputsource_base,
                                                        "ja",
                                                        "com.apple.inputmethod.Kotoeri.Japanese",
                                                        "com.apple.inputmethod.Japanese") == true);
  REQUIRE(xml_compiler.is_vk_change_inputsource_matched(vk_change_inputsource_base,
                                                        "en",
                                                        "com.apple.keylayout.US",
                                                        "") == false);
  ++vk_change_inputsource_base;

  // DVORAK
  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_CHANGE_INPUTSOURCE_DVORAK") == boost::optional<uint32_t>(vk_change_inputsource_base));
  REQUIRE(xml_compiler.is_vk_change_inputsource_matched(vk_change_inputsource_base,
                                                        "en",
                                                        "com.apple.keylayout.Dvorak",
                                                        "") == true);
  REQUIRE(xml_compiler.is_vk_change_inputsource_matched(vk_change_inputsource_base,
                                                        "en",
                                                        "com.apple.keylayout.US",
                                                        "") == false);
  ++vk_change_inputsource_base;

  // SWISS
  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_CHANGE_INPUTSOURCE_SWISS") == boost::optional<uint32_t>(vk_change_inputsource_base));
  REQUIRE(xml_compiler.is_vk_change_inputsource_matched(vk_change_inputsource_base,
                                                        "en",
                                                        "com.apple.keylayout.SwissFrench",
                                                        "") == true);
  REQUIRE(xml_compiler.is_vk_change_inputsource_matched(vk_change_inputsource_base,
                                                        "en",
                                                        "com.apple.keylayout.US",
                                                        "") == false);
  ++vk_change_inputsource_base;

  // Invalid keycode
  REQUIRE(xml_compiler.is_vk_change_inputsource_matched(vk_change_inputsource_base,
                                                        "",
                                                        "",
                                                        "") == false);

  // ------------------------------------------------------------
  // inputsourcedef
  {
    std::vector<uint32_t> expect;
    expect.push_back(*(xml_compiler.get_symbol_map().get_optional("InputSource::CANADIAN")));
    expect.push_back(*(xml_compiler.get_symbol_map().get_optional("InputSource::ENGLISH")));

    std::vector<uint32_t> actual;
    for (size_t i = 0; i < xml_compiler.get_inputsource_vector_size(); ++i) {
      uint32_t inputsource = 0;
      if (xml_compiler.is_inputsource_matched(inputsource,
                                              i,
                                              "en",
                                              "com.apple.keylayout.Canadian",
                                              "")) {
        actual.push_back(inputsource);
      }
    }

    REQUIRE(actual == expect);
  }

#if 0
  {
    uint32_t inputsource;
    xml_compiler.get_inputsourceid(inputsource,
                                   "en",
                                   "com.apple.keylayout.Canadian",
                                   "");
REQUIRE(inputsource == xml_compiler.get_symbol_map().get_optional("InputSource::CANADIAN"));
REQUIRE(xml_compiler.get_symbol_map().get_optional("InputSource::FRENCH") != inputsource == true);
  }

  {
    uint32_t inputsource;
    xml_compiler.get_inputsourceid(inputsource,
                                   "",
                                   "com.apple.keyboardlayout.fr-dvorak-bepo.keylayout.FrenchDvorak",
                                   "");
REQUIRE(inputsource == xml_compiler.get_symbol_map().get_optional("InputSource::BEPO"));
  }

  {
    uint32_t inputsource;
    xml_compiler.get_inputsourceid(inputsource,
                                   "",
                                   "com.apple.keyboardlayout.fr-dvorak-bepo.keylayout.FrenchDvorak-AzertyCmd",
                                   "");
REQUIRE(inputsource == xml_compiler.get_symbol_map().get_optional("InputSource::BEPO"));
  }

  {
    uint32_t inputsource;
    xml_compiler.get_inputsourceid(inputsource,
                                   "ja",
                                   "com.apple.inputmethod.Kotoeri.Japanese",
                                   "com.apple.inputmethod.Japanese");
REQUIRE(inputsource == xml_compiler.get_symbol_map().get_optional("InputSource::JAPANESE"));
  }

  {
    uint32_t inputsource;
    xml_compiler.get_inputsourceid(inputsource,
                                   "fr",
                                   "com.apple.keylayout.French",
                                   "");
REQUIRE(inputsource == xml_compiler.get_symbol_map().get_optional("InputSource::FRENCH"));
  }

  {
    uint32_t inputsource;
    xml_compiler.get_inputsourceid(inputsource,
                                   "en",
                                   "com.apple.keylayout.US",
                                   "");
REQUIRE(inputsource == xml_compiler.get_symbol_map().get_optional("InputSource::MY_ENGLISH"));
  }
#endif

  // ------------------------------------------------------------
  // vkopenurldef
  uint32_t vk_open_url_base = vk_change_inputsource_base;

  {
    REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_OPEN_URL_WEB_pqrs_org") == boost::optional<uint32_t>(vk_open_url_base));
    ++vk_open_url_base;
  }

  {
    REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_OPEN_URL_APP_TextEdit") == boost::optional<uint32_t>(vk_open_url_base));
    ++vk_open_url_base;
  }

  {
    REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_OPEN_URL_SHELL_date_pbcopy") == boost::optional<uint32_t>(vk_open_url_base));
    ++vk_open_url_base;
  }

  // ------------------------------------------------------------
  uint32_t vk_config_base = vk_open_url_base;
  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_CONFIG_TOGGLE_notsave_passthrough") == boost::optional<uint32_t>(vk_config_base++));
  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_CONFIG_FORCE_ON_notsave_passthrough") == boost::optional<uint32_t>(vk_config_base++));
  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_CONFIG_FORCE_OFF_notsave_passthrough") == boost::optional<uint32_t>(vk_config_base++));
  REQUIRE(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_CONFIG_SYNC_KEYDOWNUP_notsave_passthrough") == boost::optional<uint32_t>(vk_config_base++));

  REQUIRE(xml_compiler.get_symbol_map().get_optional("ConsumerKeyCode::BRIGHTNESS_UP") == boost::optional<uint32_t>(2));
  REQUIRE(*(xml_compiler.get_symbol_map().get_name("ConsumerKeyCode", 2)) == "ConsumerKeyCode::BRIGHTNESS_UP");
  REQUIRE(xml_compiler.get_symbol_map().get_name("ConsumerKeyCode", 12345) == boost::none);

  // system 6 + private 4 - duplicated 2 == 8
  REQUIRE(xml_compiler.get_app_vector_size() == 8);
  // system 70 + private 1 == 71
  REQUIRE(xml_compiler.get_inputsource_vector_size() == 71);
  // system 2 + private 0 == 2
  REQUIRE(xml_compiler.get_window_name_vector_size() == 2);

  // ---------------------------------------
  {
    std::vector<uint32_t> expect;
    expect.push_back(*(xml_compiler.get_symbol_map().get_optional("ApplicationType::TERMINAL_APPLE")));
    expect.push_back(*(xml_compiler.get_symbol_map().get_optional("ApplicationType::TERMINAL")));

    std::vector<uint32_t> actual;
    for (size_t i = 0; i < xml_compiler.get_app_vector_size(); ++i) {
      uint32_t appid = 0;
      if (xml_compiler.is_app_matched(appid, i, "com.apple.Terminal")) {
        actual.push_back(appid);
      }
    }

    REQUIRE(actual == expect);
  }

  {
    std::vector<uint32_t> expect;
    expect.push_back(*(xml_compiler.get_symbol_map().get_optional("ApplicationType::VI")));

    std::vector<uint32_t> actual;
    for (size_t i = 0; i < xml_compiler.get_app_vector_size(); ++i) {
      uint32_t appid = 0;
      if (xml_compiler.is_app_matched(appid, i, "org.vim.MacVim")) {
        actual.push_back(appid);
      }
    }

    REQUIRE(actual == expect);
  }

  {
    std::vector<uint32_t> expect;

    std::vector<uint32_t> actual;
    for (size_t i = 0; i < xml_compiler.get_app_vector_size(); ++i) {
      uint32_t appid = 0;
      if (xml_compiler.is_app_matched(appid, i, "com.googlecode.iterm2")) {
        actual.push_back(appid);
      }
    }

    REQUIRE(actual == expect);
  }

  {
    std::vector<uint32_t> expect;
    expect.push_back(*(xml_compiler.get_symbol_map().get_optional("ApplicationType::EMACS")));

    std::vector<uint32_t> actual;
    for (size_t i = 0; i < xml_compiler.get_app_vector_size(); ++i) {
      uint32_t appid = 0;
      if (xml_compiler.is_app_matched(appid, i, "org.gnu.Emacs")) {
        actual.push_back(appid);
      }
    }

    REQUIRE(actual == expect);
  }

  {
    std::vector<uint32_t> expect;

    std::vector<uint32_t> actual;
    for (size_t i = 0; i < xml_compiler.get_app_vector_size(); ++i) {
      uint32_t appid = 0;
      if (xml_compiler.is_app_matched(appid, i, "org.gnu.AquamacsEmacs")) {
        actual.push_back(appid);
      }
    }

    REQUIRE(actual == expect);
  }

  {
    std::vector<uint32_t> expect;
    expect.push_back(*(xml_compiler.get_symbol_map().get_optional("ApplicationType::PREFIX_TEST")));

    std::vector<uint32_t> actual;
    for (size_t i = 0; i < xml_compiler.get_app_vector_size(); ++i) {
      uint32_t appid = 0;
      if (xml_compiler.is_app_matched(appid, i, "org.pqrs.prefix.test")) {
        actual.push_back(appid);
      }
    }

    REQUIRE(actual == expect);
  }

  {
    std::vector<uint32_t> expect;
    expect.push_back(*(xml_compiler.get_symbol_map().get_optional("ApplicationType::PREFIX_TEST")));

    std::vector<uint32_t> actual;
    for (size_t i = 0; i < xml_compiler.get_app_vector_size(); ++i) {
      uint32_t appid = 0;
      if (xml_compiler.is_app_matched(appid, i, "org.pqrs.prefix.test1111")) {
        actual.push_back(appid);
      }
    }

    REQUIRE(actual == expect);
  }

  {
    std::vector<uint32_t> expect;

    std::vector<uint32_t> actual;
    for (size_t i = 0; i < xml_compiler.get_app_vector_size(); ++i) {
      uint32_t appid = 0;
      if (xml_compiler.is_app_matched(appid, i, "dummy.org.pqrs.prefix.test")) {
        actual.push_back(appid);
      }
    }

    REQUIRE(actual == expect);
  }

  {
    std::vector<uint32_t> expect;
    expect.push_back(*(xml_compiler.get_symbol_map().get_optional("ApplicationType::SUFFIX_TEST")));

    std::vector<uint32_t> actual;
    for (size_t i = 0; i < xml_compiler.get_app_vector_size(); ++i) {
      uint32_t appid = 0;
      if (xml_compiler.is_app_matched(appid, i, "test.suffix.pqrs.org")) {
        actual.push_back(appid);
      }
    }

    REQUIRE(actual == expect);
  }

  {
    std::vector<uint32_t> expect;
    expect.push_back(*(xml_compiler.get_symbol_map().get_optional("ApplicationType::SUFFIX_TEST")));

    std::vector<uint32_t> actual;
    for (size_t i = 0; i < xml_compiler.get_app_vector_size(); ++i) {
      uint32_t appid = 0;
      if (xml_compiler.is_app_matched(appid, i, "1111test.suffix.pqrs.org")) {
        actual.push_back(appid);
      }
    }

    REQUIRE(actual == expect);
  }

  {
    std::vector<uint32_t> expect;

    std::vector<uint32_t> actual;
    for (size_t i = 0; i < xml_compiler.get_app_vector_size(); ++i) {
      uint32_t appid = 0;
      if (xml_compiler.is_app_matched(appid, i, "test.suffix.pqrs.org.dummy")) {
        actual.push_back(appid);
      }
    }

    REQUIRE(actual == expect);
  }

  // ---------------------------------------
  {
    std::vector<uint32_t> expect;
    expect.push_back(*(xml_compiler.get_symbol_map().get_optional("InputSource::MY_ENGLISH")));
    expect.push_back(*(xml_compiler.get_symbol_map().get_optional("InputSource::ENGLISH")));

    std::vector<uint32_t> actual;
    for (size_t i = 0; i < xml_compiler.get_inputsource_vector_size(); ++i) {
      uint32_t inputsource = 0;
      if (xml_compiler.is_inputsource_matched(inputsource,
                                              i,
                                              "en",
                                              "com.apple.keylayout.US",
                                              "")) {
        actual.push_back(inputsource);
      }
    }

    REQUIRE(actual == expect);
  }

  {
    std::vector<uint32_t> expect;
    expect.push_back(*(xml_compiler.get_symbol_map().get_optional("WindowName::Gmail")));
    expect.push_back(*(xml_compiler.get_symbol_map().get_optional("WindowName::Google")));

    std::vector<uint32_t> actual;
    for (size_t i = 0; i < xml_compiler.get_window_name_vector_size(); ++i) {
      uint32_t windownameid;

      if (xml_compiler.is_window_name_matched(windownameid,
                                              i,
                                              "Example - Gmail")) {
        actual.push_back(windownameid);
      }
    }

    REQUIRE(actual == expect);
  }

  REQUIRE(xml_compiler.get_symbol_map().get_optional("ApplicationType::SPACE_IS_IGNORED") == boost::optional<uint32_t>(6));

  REQUIRE(xml_compiler.get_symbol_map().get_optional("DeviceVendor::HEWLETT_PACKARD") == boost::optional<uint32_t>(0x03f0));
  REQUIRE(xml_compiler.get_symbol_map().get_optional("DeviceVendor::SPACE_IS_IGNORED") == boost::optional<uint32_t>(0x9999));
  REQUIRE(xml_compiler.get_symbol_map().get_optional("DeviceProduct::MY_HP_KEYBOARD") == boost::optional<uint32_t>(0x0224));
  REQUIRE(xml_compiler.get_symbol_map().get_optional("DeviceProduct::SPACE_IS_IGNORED") == boost::optional<uint32_t>(0x9999));

  REQUIRE(xml_compiler.get_url(0) == boost::none);

  REQUIRE(*(xml_compiler.get_url(*(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_OPEN_URL_WEB_pqrs_org")))) == "https://pqrs.org/");
  REQUIRE(xml_compiler.get_url_type(*(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_OPEN_URL_WEB_pqrs_org"))) == boost::none);

  REQUIRE(*(xml_compiler.get_url(*(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_OPEN_URL_APP_TextEdit")))) == "file:///Applications/TextEdit.app");
  REQUIRE(xml_compiler.get_url_type(*(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_OPEN_URL_WEB_pqrs_org"))) == boost::none);

  REQUIRE(*(xml_compiler.get_url(*(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_OPEN_URL_SHELL_date_pbcopy")))) == "/bin/date | /usr/bin/pbcopy");
  REQUIRE(*(xml_compiler.get_url_type(*(xml_compiler.get_symbol_map().get_optional("KeyCode::VK_OPEN_URL_SHELL_date_pbcopy")))) == "shell");

  auto node_tree = xml_compiler.get_preferences_checkbox_node_tree();
  REQUIRE(node_tree.get_children() != nullptr);
  {
    auto node_ptr = (*(node_tree.get_children()))[0];
    REQUIRE(node_ptr->get_node().get_name() == "Swap Space and Tab\n  appendix1\n\n  appendix123\n  appendix123_2");
  }
  {
    auto node_ptr = (*(node_tree.get_children()))[1];
    REQUIRE(node_ptr->get_node().get_name() == "style test: important");
    REQUIRE(node_ptr->get_node().get_style() == "important");
  }
  {
    auto node_ptr = (*(node_tree.get_children()))[2];
    REQUIRE(node_ptr->get_node().get_name() == "style test: caution");
    REQUIRE(node_ptr->get_node().get_style() == "caution");
  }

  {
    std::vector<uint32_t> actual;
    REQUIRE(xml_compiler.debug_get_initialize_vector(actual, "remap.showstatusmessage") == true);

    std::vector<uint32_t> expected;

    // <autogen>__ShowStatusMessage__ A B C D E F G -- </autogen>

    uint32_t buffer[5];
    memset(&buffer, 0, sizeof(buffer));
    strlcpy(reinterpret_cast<char*>(buffer), "A B C D E F G --", sizeof(buffer));

    expected.push_back(6); // count
    expected.push_back(BRIDGE_STATUSMESSAGE);
    expected.push_back(buffer[0]); // "A B "
    expected.push_back(buffer[1]); // "C D "
    expected.push_back(buffer[2]); // "E F "
    expected.push_back(buffer[3]); // "G --"
    expected.push_back(buffer[4]); // \0

    REQUIRE(actual == expected);
  }

  {
    std::vector<uint32_t> actual;
    REQUIRE(xml_compiler.debug_get_initialize_vector(actual, "remap.showstatusmessage_old_style") == true);

    std::vector<uint32_t> expected;

    // <autogen>--ShowStatusMessage-- O L D 1 2 3 4 - -- </autogen>

    uint32_t buffer[6];
    memset(&buffer, 0, sizeof(buffer));
    strlcpy(reinterpret_cast<char*>(buffer), "O L D 1 2 3 4 - --", sizeof(buffer));

    expected.push_back(6); // count
    expected.push_back(BRIDGE_STATUSMESSAGE);
    expected.push_back(buffer[0]); // "O L "
    expected.push_back(buffer[1]); // "D 1 "
    expected.push_back(buffer[2]); // "2 3 "
    expected.push_back(buffer[3]); // "4 - "
    expected.push_back(buffer[4]); // "--" \0\0

    REQUIRE(actual == expected);
  }

  {
    std::vector<uint32_t> actual;
    REQUIRE(xml_compiler.debug_get_initialize_vector(actual, "remap.use_separator") == true);

    std::vector<uint32_t> expected;

    // <autogen>
    //   __KeyOverlaidModifier__
    //   KeyCode::SPACE,
    //
    //   KeyCode::RETURN, KeyCode::TAB,
    //
    //   Option::SEPARATOR,
    //
    //   KeyCode::SPACE,
    // </autogen>

    expected.push_back(13); // count
    expected.push_back(BRIDGE_REMAPTYPE_KEYOVERLAIDMODIFIER);

    expected.push_back(BRIDGE_DATATYPE_OPTION);
    expected.push_back(1); // Option::USE_SEPARATOR

    expected.push_back(BRIDGE_DATATYPE_KEYCODE);
    expected.push_back(49); // space

    expected.push_back(BRIDGE_DATATYPE_KEYCODE);
    expected.push_back(36); // return

    expected.push_back(BRIDGE_DATATYPE_KEYCODE);
    expected.push_back(48); // tab

    expected.push_back(BRIDGE_DATATYPE_OPTION);
    expected.push_back(2); // Option::SEPARATOR

    expected.push_back(BRIDGE_DATATYPE_KEYCODE);
    expected.push_back(49); // space

    REQUIRE(actual == expected);
  }

  {
    std::vector<uint32_t> actual;
    REQUIRE(xml_compiler.debug_get_initialize_vector(actual, "remap.without_separator") == true);

    std::vector<uint32_t> expected;

    // <autogen>
    //   __KeyOverlaidModifier__
    //   KeyCode::SPACE,
    //
    //   KeyCode::RETURN,
    //
    //   KeyCode::TAB, KeyCode::SPACE,
    // </autogen>

    expected.push_back(9); // count
    expected.push_back(BRIDGE_REMAPTYPE_KEYOVERLAIDMODIFIER);

    expected.push_back(BRIDGE_DATATYPE_KEYCODE);
    expected.push_back(49); // space

    expected.push_back(BRIDGE_DATATYPE_KEYCODE);
    expected.push_back(36); // return

    expected.push_back(BRIDGE_DATATYPE_KEYCODE);
    expected.push_back(48); // tab

    expected.push_back(BRIDGE_DATATYPE_KEYCODE);
    expected.push_back(49); // space

    REQUIRE(actual == expected);
  }

  // ---------------------------------------
  // override_bundle_identifier
  {
    auto actual = xml_compiler.override_bundle_identifier("com.microsoft.rdc.mac",
                                                          "Microsoft Remote Desktop",
                                                          "AXTextField");
    std::string expected_string = "karabiner.remotedesktop-preferences";
    auto expected = boost::optional<const std::string&>(expected_string);
    REQUIRE(actual == expected);
  }

  {
    auto actual = xml_compiler.override_bundle_identifier("com.google.Chrome",
                                                          "Chrome Remote Desktop",
                                                          "AXWindow");
    std::string expected_string = "karabiner.remotedesktop";
    auto expected = boost::optional<const std::string&>(expected_string);
    REQUIRE(actual == expected);
  }

  {
    auto actual = xml_compiler.override_bundle_identifier("org.pqrs.Karabiner",
                                                          "Karabiner",
                                                          "AXTextField");
    boost::optional<const std::string&> expected = boost::none;
    REQUIRE(actual == expected);
  }

  {
    // <prefix>Lorem ipsum</prefix>
    auto actual = xml_compiler.override_bundle_identifier("org.pqrs.Karabiner",
                                                          "Lorem ipsum dolor",
                                                          "AXTextField");
    std::string expected_string = "karabiner.test";
    auto expected = boost::optional<const std::string&>(expected_string);
    REQUIRE(actual == expected);
  }

  {
    // <suffix>elit</suffix>
    auto actual = xml_compiler.override_bundle_identifier("org.pqrs.Karabiner",
                                                          "consectetur adipisicing elit",
                                                          "AXTextField");
    std::string expected_string = "karabiner.test";
    auto expected = boost::optional<const std::string&>(expected_string);
    REQUIRE(actual == expected);
  }

  {
    // not <prefix> and <suffix>
    auto actual = xml_compiler.override_bundle_identifier("org.pqrs.Karabiner",
                                                          "ipsum dolor sit amet, consectetur adipisicing",
                                                          "AXTextField");
    boost::optional<const std::string&> expected = boost::none;
    REQUIRE(actual == expected);
  }

  {
    // <regex>dolor.*eiusmod tempor</regex>
    auto actual = xml_compiler.override_bundle_identifier("org.pqrs.Karabiner",
                                                          "ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua",
                                                          "AXTextField");
    std::string expected_string = "karabiner.test";
    auto expected = boost::optional<const std::string&>(expected_string);
    REQUIRE(actual == expected);
  }
}

TEST_CASE("reload_bindings_clang", "[pqrs_xml_compiler]") {
  pqrs_xml_compiler* p = nullptr;
  REQUIRE(pqrs_xml_compiler_initialize(&p, "data/system_xml", "data/private_xml") == 0);
  pqrs_xml_compiler_reload(p, "checkbox.xml");

  {
    std::vector<uint32_t> expect;
    expect.push_back(pqrs_xml_compiler_get_symbol_map_value(p, "InputSource::CANADIAN"));
    expect.push_back(pqrs_xml_compiler_get_symbol_map_value(p, "InputSource::ENGLISH"));

    std::vector<uint32_t> actual;
    for (size_t i = 0; i < pqrs_xml_compiler_get_inputsource_vector_size(p); ++i) {
      uint32_t inputsource = 0;
      if (pqrs_xml_compiler_is_inputsource_matched(p,
                                                   &inputsource,
                                                   i,
                                                   "en",
                                                   "com.apple.keylayout.Canadian",
                                                   "")) {
        actual.push_back(inputsource);
      }
    }

    REQUIRE(actual == expect);
  }

  {
    std::vector<uint32_t> expect;

    std::vector<uint32_t> actual;
    for (size_t i = 0; i < pqrs_xml_compiler_get_inputsource_vector_size(p); ++i) {
      uint32_t inputsource = 0;
      if (pqrs_xml_compiler_is_inputsource_matched(p,
                                                   &inputsource,
                                                   i,
                                                   nullptr,
                                                   nullptr,
                                                   nullptr)) {
        actual.push_back(inputsource);
      }
    }

    REQUIRE(actual == expect);
  }

  pqrs_xml_compiler_terminate(&p);
}

TEST_CASE("reload_invalid_xml", "[pqrs_xml_compiler]") {
  // ------------------------------------------------------------
  // invalid XML format
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/broken_xml");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "<data/invalid_xml/broken_xml/private.xml>(4): expected element name");
    REQUIRE(xml_compiler.get_symbol_map().get_optional("ConsumerKeyCode::BRIGHTNESS_UP") == boost::optional<uint32_t>(2));
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/broken_include");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "<data/invalid_xml/broken_include/include.xml>(4): expected element name");
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/missing_include");
    xml_compiler.reload();

    REQUIRE(xml_compiler.get_error_information().get_count() == 0);

    auto node_tree = xml_compiler.get_preferences_checkbox_node_tree();
    REQUIRE(node_tree.get_children() != nullptr);

    auto node_ptr = (*(node_tree.get_children()))[0];
    REQUIRE(node_ptr->get_node().get_name() == "Caution:\n  data/invalid_xml/missing_include/include.xml is not found.");
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/infinite_include_loop");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "An infinite include loop is detected:\n"
                                                                  "data/invalid_xml/infinite_include_loop/private.xml");
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/infinite_include_loop2");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "An infinite include loop is detected:\n"
                                                                  "data/invalid_xml/infinite_include_loop2/include.xml");
  }

  // ------------------------------------------------------------
  // identifier
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/dup_identifier");
    xml_compiler.reload();
    const char* message = "Duplicated identifier:\n"
                          "\n"
                          "<identifier>private.swap_space_and_tab</identifier>";
    REQUIRE(xml_compiler.get_error_information().get_message() == message);
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/empty_identifier");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "Empty <identifier>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 2);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/empty_identifier2");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "Empty <identifier>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 2);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/invalid_identifier_place");
    xml_compiler.reload();
    const char* message = "<identifier> must be placed directly under <item>:\n"
                          "\n"
                          "<identifier>private.swap_space_and_tab</identifier>";
    REQUIRE(xml_compiler.get_error_information().get_message() == message);
    REQUIRE(xml_compiler.get_error_information().get_count() == 2);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/invalid_identifier_place2");
    xml_compiler.reload();
    const char* message = "<identifier> must be placed directly under <item>:\n"
                          "\n"
                          "<identifier>private.swap_space_and_tab</identifier>";
    REQUIRE(xml_compiler.get_error_information().get_message() == message);
    REQUIRE(xml_compiler.get_error_information().get_count() == 2);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/invalid_identifier_place3");
    xml_compiler.reload();
    const char* message = "You should not write <identifier> in <item> which has child <item> nodes.\n"
                          "Remove <identifier>private.invalid_identifier_place3</identifier>.";
    REQUIRE(xml_compiler.get_error_information().get_message() == message);
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }

  // ------------------------------------------------------------
  // autogen
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/unknown_autogen");
    xml_compiler.reload();
    const char* message = "Invalid <autogen>:\n"
                          "\n"
                          "<autogen>__KeyToKey2__ KeyCode::SPACE, VK_SHIFT, KeyCode::TAB</autogen>";
    REQUIRE(xml_compiler.get_error_information().get_message() == message);
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);

    {
      std::vector<uint32_t> actual;
      REQUIRE(xml_compiler.debug_get_initialize_vector(actual, "private.swap_space_and_tab") == true);

      // empty if error
      std::vector<uint32_t> expected;
      REQUIRE(actual == expected);
    }
    {
      std::vector<uint32_t> actual;
      REQUIRE(xml_compiler.debug_get_initialize_vector(actual, "private.swap_space_and_tab2") == true);

      std::vector<uint32_t> expected;

      // ------------------------------------------------------------
      // <autogen>__KeyToKey__ KeyCode::SPACE, VK_SHIFT, KeyCode::TAB</autogen>
      expected.push_back(9); // count
      expected.push_back(BRIDGE_REMAPTYPE_KEYTOKEY);
      expected.push_back(BRIDGE_DATATYPE_KEYCODE);
      expected.push_back(49); // KeyCode::SPACE
      expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAG);
      expected.push_back(2); // ModifierFlag::SHIFT_L
      expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAGS_END);
      expected.push_back(1);
      expected.push_back(BRIDGE_DATATYPE_KEYCODE);
      expected.push_back(48); // KeyCode::TAB

      expected.push_back(9); // count
      expected.push_back(BRIDGE_REMAPTYPE_KEYTOKEY);
      expected.push_back(BRIDGE_DATATYPE_KEYCODE);
      expected.push_back(49); // KeyCode::SPACE
      expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAG);
      expected.push_back(3); // ModifierFlag::SHIFT_R
      expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAGS_END);
      expected.push_back(1);
      expected.push_back(BRIDGE_DATATYPE_KEYCODE);
      expected.push_back(48); // KeyCode::TAB

      // ------------------------------------------------------------
      // <autogen>--KeyToKey-- KeyCode::TAB, VK_SHIFT, KeyCode::SPACE</autogen>
      expected.push_back(9); // count
      expected.push_back(BRIDGE_REMAPTYPE_KEYTOKEY);
      expected.push_back(BRIDGE_DATATYPE_KEYCODE);
      expected.push_back(48); // KeyCode::TAB
      expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAG);
      expected.push_back(2); // ModifierFlag::SHIFT_L
      expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAGS_END);
      expected.push_back(1);
      expected.push_back(BRIDGE_DATATYPE_KEYCODE);
      expected.push_back(49); // KeyCode::SPACE

      expected.push_back(9); // count
      expected.push_back(BRIDGE_REMAPTYPE_KEYTOKEY);
      expected.push_back(BRIDGE_DATATYPE_KEYCODE);
      expected.push_back(48); // KeyCode::TAB
      expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAG);
      expected.push_back(3); // ModifierFlag::SHIFT_R
      expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAGS_END);
      expected.push_back(1);
      expected.push_back(BRIDGE_DATATYPE_KEYCODE);
      expected.push_back(49); // KeyCode::SPACE

      REQUIRE(actual == expected);
    }
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/autogen_invalid_pipe_type");
    xml_compiler.reload();
    const char* message = "Cannot connect(|) except ModifierFlag:\n"
                          "\n"
                          "KeyCode::SPACE|KeyCode::TAB";
    REQUIRE(xml_compiler.get_error_information().get_message() == message);
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }

  // ------------------------------------------------------------
  // Unknown symbol_map
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/unknown_symbol_map");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "Unknown symbol:\n\nKeyCode::MY_UNKNOWN_KEY");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }

  // ------------------------------------------------------------
  // Unknown data_type
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/unknown_data_type");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "Unknown symbol:\n\nKeyCode2::SPACE");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }

  // ------------------------------------------------------------
  // replacementdef.xml
  {
    pqrs::xml_compiler xml_compiler("data/invalid_xml/replacementdef_no_name", "data/private_xml");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "No <replacementname> within <replacementdef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/invalid_xml/replacementdef_empty_name", "data/private_xml");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "Empty <replacementname> within <replacementdef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/invalid_xml/replacementdef_invalid_name1", "data/private_xml");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "<data/invalid_xml/replacementdef_invalid_name1/replacementdef.xml>(60): unexpected end of data");
    REQUIRE(xml_compiler.get_error_information().get_count() == 2);
  }
  {
    pqrs::xml_compiler xml_compiler("data/invalid_xml/replacementdef_invalid_name2", "data/private_xml");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "Do not use '{{' and '}}' within <replacementname>:\n\nVI_}}J");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/invalid_xml/replacementdef_no_value", "data/private_xml");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "No <replacementvalue> within <replacementdef>:\n\nVI_J");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/invalid_xml/replacementdef_not_found", "data/private_xml");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "Warning - \"APPDEF_REPLACEMENT\" is not found in replacement.\n");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }

  // ------------------------------------------------------------
  // symbol_map.xml
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/symbol_map_xml_no_type");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "No 'type' Attribute within <symbol_map>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/symbol_map_xml_empty_type");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "Empty 'type' Attribute within <symbol_map>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/symbol_map_xml_no_name");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "No 'name' Attribute within <symbol_map>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/symbol_map_xml_empty_name");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "Empty 'name' Attribute within <symbol_map>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/symbol_map_xml_no_value");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "No 'value' Attribute within <symbol_map>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/symbol_map_xml_empty_value");
    xml_compiler.reload();
    REQUIRE(xml_compiler.get_error_information().get_message() == "Empty 'value' Attribute within <symbol_map>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/symbol_map_xml_invalid_value");
    xml_compiler.reload();
    const char* message = "Invalid 'value' Attribute within <symbol_map>:\n"
                          "\n"
                          "<symbol_map type=\"ConsumerKeyCode\" name=\"BRIGHTNESS_UP\" value=\"XXX\" />";
    REQUIRE(xml_compiler.get_error_information().get_message() == message);
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }

  // ------------------------------------------------------------
  // modifierdef.xml
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/modifierdef_empty");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "Empty <modifierdef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/modifierdef_invalid_notify");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "Invalid 'notify' attribute within <modifierdef>: falsee");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }

  // ------------------------------------------------------------
  // appdef.xml
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/appdef_no_name");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "No <appname> within <appdef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/appdef_empty_name");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "Empty <appname> within <appdef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }

  // ------------------------------------------------------------
  // windownamedef.xml
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/windownamedef_no_name");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "No <name> within <windownamedef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/windownamedef_empty_name");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "Empty <name> within <windownamedef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }

  // ------------------------------------------------------------
  // uielementroledef.xml
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/uielementroledef_empty_name");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "Empty <uielementroledef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }

  // ------------------------------------------------------------
  // devicevendordef.xml
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/devicevendordef_no_name");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "No <vendorname> within <devicevendordef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/devicevendordef_empty_name");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "Empty <vendorname> within <devicevendordef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/devicevendordef_no_value");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "No <vendorid> within <devicevendordef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/devicevendordef_empty_value");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "Empty <vendorid> within <devicevendordef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/devicevendordef_invalid_value");
    xml_compiler.reload();
    const char* message = "Invalid <vendorid> within <devicevendordef>:\n"
                          "\n"
                          "<vendorid>XXX</vendorid>";
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == message);
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }

  // ------------------------------------------------------------
  // deviceproductdef.xml
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/deviceproductdef_no_name");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "No <productname> within <deviceproductdef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/deviceproductdef_empty_name");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "Empty <productname> within <deviceproductdef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/deviceproductdef_no_value");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "No <productid> within <deviceproductdef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/deviceproductdef_empty_value");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "Empty <productid> within <deviceproductdef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/deviceproductdef_invalid_value");
    xml_compiler.reload();
    const char* message = "Invalid <productid> within <deviceproductdef>:\n"
                          "\n"
                          "<productid>XXX</productid>";
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == message);
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }

  // ------------------------------------------------------------
  // inputsourcedef.xml
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/vkchangeinputsourcedef_no_name");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "No <name> within <vkchangeinputsourcedef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/vkchangeinputsourcedef_empty_name");
    xml_compiler.reload();
    const char* expected = "<name> within <vkchangeinputsourcedef> must start with \"KeyCode::VK_CHANGE_INPUTSOURCE_\":\n"
                           "\n"
                           "<name></name>";
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == expected);

    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }

  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/inputsourcedef_no_name");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "No <name> within <inputsourcedef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/inputsourcedef_empty_name");
    xml_compiler.reload();
    REQUIRE(std::string(xml_compiler.get_error_information().get_message()) == "Empty <name> within <inputsourcedef>.");
    REQUIRE(xml_compiler.get_error_information().get_count() == 1);
  }
}

TEST_CASE("symbol_map.add", "[pqrs_xml_compiler_symbol_map]") {
  pqrs::xml_compiler::symbol_map s;
  s.add("KeyCode", "SPACE", 36);
  s.add("KeyCode", "VK__AUTOINDEX__BEGIN__", 1024);
  s.add("KeyCode", "VK_NEW1");
  s.add("KeyCode", "VK_NEW2");

  REQUIRE(s.get("KeyCode::VK_NEW1") == static_cast<uint32_t>(1024));
  REQUIRE(s.get("KeyCode::VK_NEW2") == static_cast<uint32_t>(1025));
}

TEST_CASE("get", "[pqrs_xml_compiler_remapclasses_initialize_vector]") {
  pqrs::xml_compiler::remapclasses_initialize_vector v;
  REQUIRE(v.get().size() == 1);
}

TEST_CASE("remapclasses_initialize_vector.add", "[pqrs_xml_compiler_remapclasses_initialize_vector]") {
  pqrs::xml_compiler::remapclasses_initialize_vector v;
  v.clear();
  v.start(1, "remap.empty");
  v.end();

  v.start(3, "remap.three_items");
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.end();

  v.freeze();

  std::vector<uint32_t> expected;
  expected.push_back(4); // count is {0,1,2,3}

  expected.push_back(1); // configindex:1
  expected.push_back(1);

  expected.push_back(4); // configindex:3
  expected.push_back(3);
  expected.push_back(1);
  expected.push_back(2);
  expected.push_back(3);

  expected.push_back(1); // configindex:0
  expected.push_back(0);

  expected.push_back(1); // configindex:2
  expected.push_back(2);

  REQUIRE(v.get() == expected);
}

TEST_CASE("add_partial", "[pqrs_xml_compiler_remapclasses_initialize_vector]") {
  pqrs::xml_compiler::remapclasses_initialize_vector v;
  v.clear();
  v.start(1, "remap.empty");
  v.end();

  v.start(3, "remap.three_items");
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  // v.end();

  v.start(4, "remap.two_items");
  v.push_back(1);
  v.push_back(2);
  v.end();

  v.freeze();

  std::vector<uint32_t> expected;
  expected.push_back(5); // count is {0,1,2,3,4}

  expected.push_back(1); // configindex:1
  expected.push_back(1);

  expected.push_back(3); // configindex:4
  expected.push_back(4);
  expected.push_back(1);
  expected.push_back(2);

  expected.push_back(1); // configindex:0
  expected.push_back(0);

  expected.push_back(1); // configindex:2
  expected.push_back(2);

  expected.push_back(1); // configindex:3
  expected.push_back(3);

  REQUIRE(v.get() == expected);
}

TEST_CASE("filter_vector", "[pqrs_xml_compiler_filter_vector]") {
  pqrs::xml_compiler::symbol_map s;
  s.add("ApplicationType", "APP1", 1);
  s.add("ApplicationType", "APP2", 2);
  s.add("ApplicationType", "APP3", 3);
  s.add("WindowName", "WN1", 5);
  s.add("WindowName", "WN2", 6);
  s.add("WindowName", "WN3", 7);
  s.add("UIElementRole", "UER1", 20001);
  s.add("UIElementRole", "UER2", 20002);
  s.add("UIElementRole", "UER3", 20003);
  s.add("DeviceVendor", "VENDOR1", 10);
  s.add("DeviceVendor", "VENDOR2", 20);
  s.add("DeviceVendor", "VENDOR3", 30);
  s.add("DeviceProduct", "ANY", 0);
  s.add("DeviceProduct", "PRODUCT1", 100);
  s.add("DeviceProduct", "PRODUCT2", 200);
  s.add("DeviceProduct", "PRODUCT3", 300);
  s.add("DeviceLocation", "ANY", 0);
  s.add("DeviceLocation", "LOCATION1", 400);
  s.add("DeviceLocation", "LOCATION2", 500);
  s.add("DeviceLocation", "LOCATION3", 600);
  s.add("InputSource", "NONE", 0);
  s.add("InputSource", "INPUTSOURCE1", 110);
  s.add("InputSource", "INPUTSOURCE2", 111);
  s.add("InputSource", "INPUTSOURCE3", 112);
  s.add("ConfigIndex", "config1", 1000);
  s.add("ConfigIndex", "config2", 2000);
  s.add("ConfigIndex", "config3", 3000);
  s.add("ModifierFlag", "MOD1", 0x1000);
  s.add("ModifierFlag", "MOD2", 0x2000);
  s.add("ModifierFlag", "MOD3", 0x4000);
  s.add("KeyCode", "KC1", 4000);
  s.add("KeyCode", "KC2", 5000);
  s.add("KeyCode", "KC3", 6000);
  s.add("EventType", "DOWN", 10);
  s.add("EventType", "UP", 11);

  std::string xml("<?xml version=\"1.0\"?>"
                  "<item>"
                  "  <only>APP1,APP3</only>"
                  "  <only><!-- empty --></only>"
                  "  <only>,,</only>"
                  "  <not><!-- XXX --->APP2</not>"
                  "  <windowname_only>WN1,WN2</windowname_only>"
                  "  <windowname_not>WN3</windowname_not>"
                  "  <uielementrole_only>UER1,UER2</uielementrole_only>"
                  "  <uielementrole_not>UER3</uielementrole_not>"
                  "  <identifier>sample</identifier>"
                  "  <device_only>DeviceVendor::VENDOR1, DeviceProduct::PRODUCT1, DeviceLocation::LOCATION1</device_only>"
                  "  <device_not>"
                  "    DeviceVendor::VENDOR3,,,,"
                  "    DeviceProduct::PRODUCT3,"
                  "    DeviceVendor::VENDOR3,"
                  "  </device_not>"
                  "  <device_not></device_not>"
                  "  <deviceexists_only>DeviceVendor::VENDOR1, DeviceProduct::PRODUCT1, DeviceLocation::LOCATION1</deviceexists_only>"
                  "  <deviceexists_not>"
                  "    DeviceVendor::VENDOR3,,,,"
                  "    DeviceProduct::PRODUCT3,"
                  "    DeviceVendor::VENDOR3,"
                  "  </deviceexists_not>"
                  "  <inputsource_only>INPUTSOURCE1</inputsource_only>"
                  "  <inputsource_not>INPUTSOURCE2, INPUTSOURCE3</inputsource_not>"
                  "  <inputsourcedetail_only>INPUTSOURCE1</inputsourcedetail_only>"
                  "  <inputsourcedetail_not>INPUTSOURCE2, INPUTSOURCE3</inputsourcedetail_not>"
                  "  <inputmode_only>INPUTSOURCE1</inputmode_only>"
                  "  <inputmode_not>INPUTSOURCE2, INPUTSOURCE3</inputmode_not>"
                  "  <inputmodedetail_only>INPUTSOURCE1</inputmodedetail_only>"
                  "  <inputmodedetail_not>INPUTSOURCE2, INPUTSOURCE3</inputmodedetail_not>"
                  "  <config_only>config1,config2</config_only>"
                  "  <config_not>config3</config_not>"
                  "  <modifier_only>ModifierFlag::MOD1 ||| ModifierFlag::MOD3</modifier_only>"
                  "  <modifier_not> ModifierFlag::MOD2 </modifier_not>"
                  "  <modifierlocked_only>ModifierFlag::MOD1 ||| ModifierFlag::MOD3</modifierlocked_only>"
                  "  <modifierlocked_not> ModifierFlag::MOD2 </modifierlocked_not>"
                  "  <modifierstuck_only>ModifierFlag::MOD1 ||| ModifierFlag::MOD3</modifierstuck_only>"
                  "  <modifierstuck_not> ModifierFlag::MOD2 </modifierstuck_not>"
                  "  <lastpressedphysicalkey_not>KeyCode::KC1</lastpressedphysicalkey_not>"
                  "  <lastpressedphysicalkey_only>KeyCode::KC2</lastpressedphysicalkey_only>"
                  "  <lastreleasedphysicalkey_not>KeyCode::KC1</lastreleasedphysicalkey_not>"
                  "  <lastreleasedphysicalkey_only>KeyCode::KC2</lastreleasedphysicalkey_only>"
                  "  <lastsentevent_not>KeyCode::KC1, EventType::DOWN</lastsentevent_not>"
                  "  <lastsentevent_only>KeyCode::KC2, EventType::UP</lastsentevent_only>"
                  "  <elapsedtimesincelastpressed_greaterthan>"
                  "    Millisecond::RawValue::1000"
                  "  </elapsedtimesincelastpressed_greaterthan>"
                  "  <elapsedtimesincelastpressed_lessthan>"
                  "    Millisecond::RawValue::500"
                  "  </elapsedtimesincelastpressed_lessthan>"
                  "  <elapsedtimesincelastreleased_greaterthan>"
                  "    Millisecond::RawValue::1000"
                  "  </elapsedtimesincelastreleased_greaterthan>"
                  "  <elapsedtimesincelastreleased_lessthan>"
                  "    Millisecond::RawValue::500"
                  "  </elapsedtimesincelastreleased_lessthan>"
                  "  <pressingphysicalkeys_greaterthan>1</pressingphysicalkeys_greaterthan>"
                  "  <pressingphysicalkeys_lessthan>2</pressingphysicalkeys_lessthan>"
                  "</item>");
  std::stringstream istream(xml, std::stringstream::in);

  pqrs::xml_compiler xml_compiler("data/system_xml", "data/private_xml");

  int flags = boost::property_tree::xml_parser::no_comments;
  boost::property_tree::ptree pt;
  boost::property_tree::read_xml(istream, pt, flags);

  pqrs::string::replacement replacement;

  for (auto& it : pt) {
    pqrs::xml_compiler::filter_vector fv(s);
    fv.traverse(pqrs::xml_compiler::extracted_ptree(xml_compiler, replacement, it.second, ""));

    std::vector<uint32_t> expected;

    // <only>APP1,APP3</only>
    expected.push_back(3); // count
    expected.push_back(BRIDGE_FILTERTYPE_APPLICATION_ONLY);
    expected.push_back(1); // APP1
    expected.push_back(3); // APP3

    // <only><!-- empty --></only>"
    // <only>,,</only>"
    // ***IGNORED***

    // <not>APP2</not>
    expected.push_back(2); // count
    expected.push_back(BRIDGE_FILTERTYPE_APPLICATION_NOT);
    expected.push_back(2); // APP2

    // <windowname_only>WN1,WN2</windowname_only>
    expected.push_back(3); // count
    expected.push_back(BRIDGE_FILTERTYPE_WINDOWNAME_ONLY);
    expected.push_back(5); // WN1
    expected.push_back(6); // WN2

    // <windowname_not>WN3</windowname_not>
    expected.push_back(2); // count
    expected.push_back(BRIDGE_FILTERTYPE_WINDOWNAME_NOT);
    expected.push_back(7); // WN3

    // <uielementrole_only>UER1,UER2</uielementrole_only>
    expected.push_back(3); // count
    expected.push_back(BRIDGE_FILTERTYPE_UIELEMENTROLE_ONLY);
    expected.push_back(20001); // UER1
    expected.push_back(20002); // UER2

    // <uielementrole_not>UER3</uielementrole_not>
    expected.push_back(2); // count
    expected.push_back(BRIDGE_FILTERTYPE_UIELEMENTROLE_NOT);
    expected.push_back(20003); // UER3

    // <device_only>DeviceVendor::VENDOR1, DeviceProduct::PRODUCT1, DeviceLocation::LOCATION1</device_only>
    expected.push_back(4); // count
    expected.push_back(BRIDGE_FILTERTYPE_DEVICE_ONLY);
    expected.push_back(10);
    expected.push_back(100);
    expected.push_back(400);

    // <device_not>DeviceVendor::VENDOR3, DeviceProduct::PRODUCT3, DeviceVendor::VENDOR3</device_not>
    expected.push_back(7); // count
    expected.push_back(BRIDGE_FILTERTYPE_DEVICE_NOT);
    expected.push_back(30);
    expected.push_back(300);
    expected.push_back(0); // DeviceLocation::ANY
    expected.push_back(30);
    expected.push_back(0); // DeviceProduct::ANY
    expected.push_back(0); // DeviceLocation::ANY

    // <device_not></device_not>
    // ***IGNORED***

    // <deviceexists_only>DeviceVendor::VENDOR1, DeviceProduct::PRODUCT1, DeviceLocation::LOCATION1</deviceexists_only>
    expected.push_back(4); // count
    expected.push_back(BRIDGE_FILTERTYPE_DEVICEEXISTS_ONLY);
    expected.push_back(10);
    expected.push_back(100);
    expected.push_back(400);

    // <deviceexists_not>DeviceVendor::VENDOR3, DeviceProduct::PRODUCT3, DeviceVendor::VENDOR3</deviceexists_not>
    expected.push_back(7); // count
    expected.push_back(BRIDGE_FILTERTYPE_DEVICEEXISTS_NOT);
    expected.push_back(30);
    expected.push_back(300);
    expected.push_back(0); // DeviceLocation::ANY
    expected.push_back(30);
    expected.push_back(0); // DeviceProduct::ANY
    expected.push_back(0); // DeviceLocation::ANY

    // <inputsource_only>InputSource::INPUTSOURCE1</inputsource_only>
    expected.push_back(2); // count
    expected.push_back(BRIDGE_FILTERTYPE_INPUTSOURCE_ONLY);
    expected.push_back(110);

    // <inputsource_not>InputSource::INPUTSOURCE2, InputSource::INPUTSOURCE3</inputsource_not>
    expected.push_back(3); // count
    expected.push_back(BRIDGE_FILTERTYPE_INPUTSOURCE_NOT);
    expected.push_back(111);
    expected.push_back(112);

    // <inputsourcedetail_only>InputSource::INPUTSOURCE1</inputsourcedetail_only>
    expected.push_back(2); // count
    expected.push_back(BRIDGE_FILTERTYPE_INPUTSOURCE_ONLY);
    expected.push_back(110);

    // <inputsourcedetail_not>InputSource::INPUTSOURCE2, InputSource::INPUTSOURCE3</inputsourcedetail_not>
    expected.push_back(3); // count
    expected.push_back(BRIDGE_FILTERTYPE_INPUTSOURCE_NOT);
    expected.push_back(111);
    expected.push_back(112);

    // <inputmode_only>INPUTSOURCE1</inputmode_only>
    expected.push_back(2); // count
    expected.push_back(BRIDGE_FILTERTYPE_INPUTSOURCE_ONLY);
    expected.push_back(110);

    // <inputmode_not>INPUTSOURCE2, INPUTSOURCE3</inputmode_not>
    expected.push_back(3); // count
    expected.push_back(BRIDGE_FILTERTYPE_INPUTSOURCE_NOT);
    expected.push_back(111);
    expected.push_back(112);

    // <inputmodedetail_only>INPUTSOURCE1</inputmodedetail_only>
    expected.push_back(2); // count
    expected.push_back(BRIDGE_FILTERTYPE_INPUTSOURCE_ONLY);
    expected.push_back(110);

    // <inputmodedetail_not>INPUTSOURCE2, INPUTSOURCE3</inputmodedetail_not>
    expected.push_back(3); // count
    expected.push_back(BRIDGE_FILTERTYPE_INPUTSOURCE_NOT);
    expected.push_back(111);
    expected.push_back(112);

    // <config_only>config1,config2</config_only>
    expected.push_back(3); // count
    expected.push_back(BRIDGE_FILTERTYPE_CONFIG_ONLY);
    expected.push_back(1000);
    expected.push_back(2000);

    // <config_not>config3</config_not>
    expected.push_back(2); // count
    expected.push_back(BRIDGE_FILTERTYPE_CONFIG_NOT);
    expected.push_back(3000);

    // <modifier_only>ModifierFlag::MOD1 ||| ModifierFlag::MOD3</modifier_only>
    expected.push_back(7);
    expected.push_back(BRIDGE_FILTERTYPE_MODIFIER_ONLY);
    expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAG);
    expected.push_back(0x1000);
    expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAG);
    expected.push_back(0x4000);
    expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAGS_END);
    expected.push_back(1);

    // <modifier_not> ModifierFlag::MOD2 </modifier_not>
    expected.push_back(5);
    expected.push_back(BRIDGE_FILTERTYPE_MODIFIER_NOT);
    expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAG);
    expected.push_back(0x2000);
    expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAGS_END);
    expected.push_back(1);

    // <modifierlocked_only>ModifierFlag::MOD1 ||| ModifierFlag::MOD3</modifierlocked_only>
    expected.push_back(7);
    expected.push_back(BRIDGE_FILTERTYPE_MODIFIER_LOCKED_ONLY);
    expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAG);
    expected.push_back(0x1000);
    expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAG);
    expected.push_back(0x4000);
    expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAGS_END);
    expected.push_back(1);

    // <modifierlocked_not> ModifierFlag::MOD2 </modifierlocked_not>
    expected.push_back(5);
    expected.push_back(BRIDGE_FILTERTYPE_MODIFIER_LOCKED_NOT);
    expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAG);
    expected.push_back(0x2000);
    expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAGS_END);
    expected.push_back(1);

    // <modifierstuck_only>ModifierFlag::MOD1 ||| ModifierFlag::MOD3</modifierstuck_only>
    expected.push_back(7);
    expected.push_back(BRIDGE_FILTERTYPE_MODIFIER_STUCK_ONLY);
    expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAG);
    expected.push_back(0x1000);
    expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAG);
    expected.push_back(0x4000);
    expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAGS_END);
    expected.push_back(1);

    // <modifierstuck_not> ModifierFlag::MOD2 </modifierstuck_not>
    expected.push_back(5);
    expected.push_back(BRIDGE_FILTERTYPE_MODIFIER_STUCK_NOT);
    expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAG);
    expected.push_back(0x2000);
    expected.push_back(BRIDGE_DATATYPE_MODIFIERFLAGS_END);
    expected.push_back(1);

    // <lastpressedphysicalkey_not>KeyCode::KC1</lastpressedphysicalkey_not>
    expected.push_back(3);
    expected.push_back(BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_NOT);
    expected.push_back(BRIDGE_DATATYPE_KEYCODE);
    expected.push_back(4000);

    // <lastpressedphysicalkey_only>KeyCode::KC2</lastpressedphysicalkey_only>
    expected.push_back(3);
    expected.push_back(BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_ONLY);
    expected.push_back(BRIDGE_DATATYPE_KEYCODE);
    expected.push_back(5000);

    // <lastreleasedphysicalkey_not>KeyCode::KC1</lastreleasedphysicalkey_not>
    expected.push_back(3);
    expected.push_back(BRIDGE_FILTERTYPE_LASTRELEASEDPHYSICALKEY_NOT);
    expected.push_back(BRIDGE_DATATYPE_KEYCODE);
    expected.push_back(4000);

    // <lastreleasedphysicalkey_only>KeyCode::KC2</lastreleasedphysicalkey_only>
    expected.push_back(3);
    expected.push_back(BRIDGE_FILTERTYPE_LASTRELEASEDPHYSICALKEY_ONLY);
    expected.push_back(BRIDGE_DATATYPE_KEYCODE);
    expected.push_back(5000);

    // <lastsentevent_not>KeyCode::KC1, EventType::DOWN</lastsentevent_not>
    expected.push_back(5);
    expected.push_back(BRIDGE_FILTERTYPE_LASTSENTEVENT_NOT);
    expected.push_back(BRIDGE_DATATYPE_KEYCODE);
    expected.push_back(4000);
    expected.push_back(BRIDGE_DATATYPE_EVENTTYPE);
    expected.push_back(10);

    // <lastsentevent_only>KeyCode::KC2, EventType::UP</lastsentevent_only>
    expected.push_back(5);
    expected.push_back(BRIDGE_FILTERTYPE_LASTSENTEVENT_ONLY);
    expected.push_back(BRIDGE_DATATYPE_KEYCODE);
    expected.push_back(5000);
    expected.push_back(BRIDGE_DATATYPE_EVENTTYPE);
    expected.push_back(11);

    // <elapsedtimesincelastpressed_greaterthan>
    //   Millisecond::RawValue::1000
    // </elapsedtimesincelastpressed_greaterthan>
    expected.push_back(3);
    expected.push_back(BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_GREATERTHAN);
    expected.push_back(BRIDGE_DATATYPE_MILLISECOND);
    expected.push_back(1000);

    // <elapsedtimesincelastpressed_lessthan>
    //   Millisecond::RawValue::500
    // </elapsedtimesincelastpressed_lessthan>
    expected.push_back(3);
    expected.push_back(BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_LESSTHAN);
    expected.push_back(BRIDGE_DATATYPE_MILLISECOND);
    expected.push_back(500);

    // <elapsedtimesincelastreleased_greaterthan>
    //   Millisecond::RawValue::1000
    // </elapsedtimesincelastreleased_greaterthan>
    expected.push_back(3);
    expected.push_back(BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTRELEASED_GREATERTHAN);
    expected.push_back(BRIDGE_DATATYPE_MILLISECOND);
    expected.push_back(1000);

    // <elapsedtimesincelastreleased_lessthan>
    //   Millisecond::RawValue::500
    // </elapsedtimesincelastreleased_lessthan>
    expected.push_back(3);
    expected.push_back(BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTRELEASED_LESSTHAN);
    expected.push_back(BRIDGE_DATATYPE_MILLISECOND);
    expected.push_back(500);

    //  <pressingphysicalkeys_greaterthan>1</pressingphysicalkeys_greaterthan>"
    expected.push_back(2);
    expected.push_back(BRIDGE_FILTERTYPE_PRESSINGPHYSICALKEYS_GREATERTHAN);
    expected.push_back(1);

    //  <pressingphysicalkeys_lessthan>2</pressingphysicalkeys_lessthan>"
    expected.push_back(2);
    expected.push_back(BRIDGE_FILTERTYPE_PRESSINGPHYSICALKEYS_LESSTHAN);
    expected.push_back(2);

    REQUIRE(fv.get() == expected);
  }
}

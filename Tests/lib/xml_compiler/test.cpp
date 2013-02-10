#include <boost/property_tree/xml_parser.hpp>
#include <gtest/gtest.h>
#include "pqrs/xml_compiler_bindings_clang.h"
#include "pqrs/xml_compiler.hpp"
#include "bridge.h"

TEST(pqrs_xml_compiler, reload)
{
  pqrs::xml_compiler xml_compiler("data/system_xml", "data/private_xml");
  xml_compiler.reload();
  EXPECT_EQ(0, xml_compiler.get_error_information().get_count());
  EXPECT_EQ("", xml_compiler.get_error_information().get_message());

  {
    int v = 0;
    int space_is_ignored = 0;

    EXPECT_EQ(boost::optional<uint32_t>(v++), xml_compiler.get_symbol_map_value("ConfigIndex::system_vk_change_inputsource_definition"));
    EXPECT_EQ(boost::optional<uint32_t>(v++), xml_compiler.get_symbol_map_value("ConfigIndex::system_vk_open_url_definition"));
    EXPECT_EQ(boost::optional<uint32_t>(v++), xml_compiler.get_symbol_map_value("ConfigIndex::notsave_private_sample"));
    EXPECT_EQ(boost::optional<uint32_t>(v++), xml_compiler.get_symbol_map_value("ConfigIndex::notsave_passthrough"));
    EXPECT_EQ(boost::optional<uint32_t>(v++), xml_compiler.get_symbol_map_value("ConfigIndex::notsave_remap_sample"));
    EXPECT_EQ(boost::optional<uint32_t>(v++), xml_compiler.get_symbol_map_value("ConfigIndex::private_include_test"));
    EXPECT_EQ(boost::optional<uint32_t>(v++), xml_compiler.get_symbol_map_value("ConfigIndex::private_replacement"));
    space_is_ignored = v;
    EXPECT_EQ(boost::optional<uint32_t>(v++), xml_compiler.get_symbol_map_value("ConfigIndex::private_space_is_ignored"));

    std::string expected = "private.space_ is_ ignored";
    EXPECT_EQ(boost::optional<const std::string&>(expected), xml_compiler.get_identifier(space_is_ignored));
  }

  EXPECT_EQ(boost::optional<uint32_t>(123), xml_compiler.get_symbol_map_value("KeyCode::MY_INCLUDE_TEST_123"));
  EXPECT_EQ(boost::optional<uint32_t>(456), xml_compiler.get_symbol_map_value("KeyCode::MY_INCLUDE_TEST_456"));
  EXPECT_EQ(boost::optional<uint32_t>(654), xml_compiler.get_symbol_map_value("KeyCode::MY_INCLUDE_TEST_654"));
  EXPECT_EQ(boost::optional<uint32_t>(1), xml_compiler.get_symbol_map_value("KeyCode::MY_INCLUDE_TEST_PARENT1"));
  EXPECT_EQ(boost::optional<uint32_t>(2), xml_compiler.get_symbol_map_value("KeyCode::MY_INCLUDE_TEST_PARENT2"));
  EXPECT_EQ(boost::optional<uint32_t>(3), xml_compiler.get_symbol_map_value("KeyCode::MY_INCLUDE_TEST_PARENT3"));
  EXPECT_EQ(boost::optional<uint32_t>(123), xml_compiler.get_symbol_map_value("KeyCode::MY_LANG_KEY"));
  EXPECT_EQ(boost::optional<uint32_t>(999), xml_compiler.get_symbol_map_value("KeyCode::SPACE_IS_IGNORED"));

  // ------------------------------------------------------------
  uint32_t vk_change_inputsource_base = 1191;

  // JAPANESE
  EXPECT_EQ(boost::optional<uint32_t>(vk_change_inputsource_base),
            xml_compiler.get_symbol_map_value("KeyCode::VK_CHANGE_INPUTSOURCE_JAPANESE"));
  EXPECT_EQ(true,
            xml_compiler.is_vk_change_inputsource_matched(vk_change_inputsource_base,
                                                          "ja",
                                                          "com.apple.inputmethod.Kotoeri.Japanese",
                                                          "com.apple.inputmethod.Japanese"));
  EXPECT_EQ(false,
            xml_compiler.is_vk_change_inputsource_matched(vk_change_inputsource_base,
                                                          "en",
                                                          "com.apple.keylayout.US",
                                                          ""));
  ++vk_change_inputsource_base;

  // DVORAK
  EXPECT_EQ(boost::optional<uint32_t>(vk_change_inputsource_base),
            xml_compiler.get_symbol_map_value("KeyCode::VK_CHANGE_INPUTSOURCE_DVORAK"));
  EXPECT_EQ(true,
            xml_compiler.is_vk_change_inputsource_matched(vk_change_inputsource_base,
                                                          "en",
                                                          "com.apple.keylayout.Dvorak",
                                                          ""));
  EXPECT_EQ(false,
            xml_compiler.is_vk_change_inputsource_matched(vk_change_inputsource_base,
                                                          "en",
                                                          "com.apple.keylayout.US",
                                                          ""));
  ++vk_change_inputsource_base;

  // SWISS
  EXPECT_EQ(boost::optional<uint32_t>(vk_change_inputsource_base),
            xml_compiler.get_symbol_map_value("KeyCode::VK_CHANGE_INPUTSOURCE_SWISS"));
  EXPECT_EQ(true,
            xml_compiler.is_vk_change_inputsource_matched(vk_change_inputsource_base,
                                                          "en",
                                                          "com.apple.keylayout.SwissFrench",
                                                          ""));
  EXPECT_EQ(false,
            xml_compiler.is_vk_change_inputsource_matched(vk_change_inputsource_base,
                                                          "en",
                                                          "com.apple.keylayout.US",
                                                          ""));
  ++vk_change_inputsource_base;

  // Invalid keycode
  EXPECT_EQ(false,
            xml_compiler.is_vk_change_inputsource_matched(vk_change_inputsource_base,
                                                          "",
                                                          "",
                                                          ""));

  // ------------------------------------------------------------
  // inputsourcedef
  {
    uint32_t inputsource;
    uint32_t inputsource_detail;
    xml_compiler.get_inputsourceid(inputsource,
                                   inputsource_detail,
                                   "en",
                                   "com.apple.keylayout.Canadian",
                                   "");
    EXPECT_EQ(xml_compiler.get_symbol_map_value("InputSource::CANADIAN"), inputsource);
    EXPECT_EQ(xml_compiler.get_symbol_map_value("InputSourceDetail::CANADIAN"), inputsource_detail);

    EXPECT_TRUE(xml_compiler.get_symbol_map_value("InputSource::FRENCH") != inputsource);
    EXPECT_TRUE(xml_compiler.get_symbol_map_value("InputSourceDetail::FRENCH") != inputsource_detail);
  }

  {
    uint32_t inputsource;
    uint32_t inputsource_detail;
    xml_compiler.get_inputsourceid(inputsource, inputsource_detail,
                                   "",
                                   "com.apple.keyboardlayout.fr-dvorak-bepo.keylayout.FrenchDvorak",
                                   "");
    EXPECT_EQ(xml_compiler.get_symbol_map_value("InputSource::BEPO"), inputsource);
    EXPECT_EQ(xml_compiler.get_symbol_map_value("InputSourceDetail::BEPO"), inputsource_detail);
  }

  {
    uint32_t inputsource;
    uint32_t inputsource_detail;
    xml_compiler.get_inputsourceid(inputsource, inputsource_detail,
                                   "",
                                   "com.apple.keyboardlayout.fr-dvorak-bepo.keylayout.FrenchDvorak-AzertyCmd",
                                   "");
    EXPECT_EQ(xml_compiler.get_symbol_map_value("InputSource::BEPO"), inputsource);
    EXPECT_EQ(xml_compiler.get_symbol_map_value("InputSourceDetail::BEPO_AZERTYCMD"), inputsource_detail);
  }

  {
    uint32_t inputsource;
    uint32_t inputsource_detail;
    xml_compiler.get_inputsourceid(inputsource, inputsource_detail,
                                   "ja",
                                   "com.apple.inputmethod.Kotoeri.Japanese",
                                   "com.apple.inputmethod.Japanese");
    EXPECT_EQ(xml_compiler.get_symbol_map_value("InputSource::JAPANESE"), inputsource);
    EXPECT_EQ(xml_compiler.get_symbol_map_value("InputSourceDetail::JAPANESE"), inputsource_detail);
  }

  {
    uint32_t inputsource;
    uint32_t inputsource_detail;
    xml_compiler.get_inputsourceid(inputsource, inputsource_detail,
                                   "fr",
                                   "com.apple.keylayout.French",
                                   "");
    EXPECT_EQ(xml_compiler.get_symbol_map_value("InputSource::FRENCH"), inputsource);
    EXPECT_EQ(xml_compiler.get_symbol_map_value("InputSourceDetail::FRENCH"), inputsource_detail);
  }

  // ------------------------------------------------------------
  // vkopenurldef
  uint32_t vk_open_url_base = vk_change_inputsource_base;

  {
    EXPECT_EQ(boost::optional<uint32_t>(vk_open_url_base),
              xml_compiler.get_symbol_map_value("KeyCode::VK_OPEN_URL_HTTP_PQRS_ORG"));
    ++vk_open_url_base;
  }

  {
    EXPECT_EQ(boost::optional<uint32_t>(vk_open_url_base),
              xml_compiler.get_symbol_map_value("KeyCode::VK_OPEN_URL_APP_TEXTEDIT"));
    ++vk_open_url_base;
  }

  // ------------------------------------------------------------
  uint32_t vk_config_base = vk_open_url_base;
  EXPECT_EQ(boost::optional<uint32_t>(vk_config_base++),
            xml_compiler.get_symbol_map_value("KeyCode::VK_CONFIG_TOGGLE_notsave_passthrough"));
  EXPECT_EQ(boost::optional<uint32_t>(vk_config_base++),
            xml_compiler.get_symbol_map_value("KeyCode::VK_CONFIG_FORCE_ON_notsave_passthrough"));
  EXPECT_EQ(boost::optional<uint32_t>(vk_config_base++),
            xml_compiler.get_symbol_map_value("KeyCode::VK_CONFIG_FORCE_OFF_notsave_passthrough"));
  EXPECT_EQ(boost::optional<uint32_t>(vk_config_base++),
            xml_compiler.get_symbol_map_value("KeyCode::VK_CONFIG_SYNC_KEYDOWNUP_notsave_passthrough"));

  EXPECT_EQ(boost::optional<uint32_t>(2), xml_compiler.get_symbol_map_value("ConsumerKeyCode::BRIGHTNESS_UP"));

  EXPECT_EQ(boost::optional<uint32_t>(5), xml_compiler.get_symbol_map_value("ApplicationType::VI"));
  EXPECT_EQ(xml_compiler.get_symbol_map_value("ApplicationType::VI"),
            xml_compiler.get_appid("org.vim.MacVim"));

  EXPECT_EQ(boost::optional<uint32_t>(7), xml_compiler.get_symbol_map_value("ApplicationType::SPACE_IS_IGNORED"));

  // com.apple.Terminal is overwritten by private.xml.
  EXPECT_EQ(xml_compiler.get_symbol_map_value("ApplicationType::TERMINAL_APPLE"),
            xml_compiler.get_appid("com.apple.Terminal"));
  EXPECT_EQ(xml_compiler.get_symbol_map_value("ApplicationType::TERMINAL"),
            xml_compiler.get_appid("com.googlecode.iterm2"));

  // org.gnu.Emacs is overwritten by private.xml.
  EXPECT_EQ(xml_compiler.get_symbol_map_value("ApplicationType::EMACS"),
            xml_compiler.get_appid("org.gnu.Emacs"));
  EXPECT_EQ(xml_compiler.get_symbol_map_value("ApplicationType::EMACS"),
            xml_compiler.get_appid("org.gnu.AquamacsEmacs"));

  EXPECT_EQ(xml_compiler.get_symbol_map_value("ApplicationType::PREFIX_TEST"),
            xml_compiler.get_appid("org.pqrs.prefix.test"));
  EXPECT_EQ(xml_compiler.get_symbol_map_value("ApplicationType::PREFIX_TEST"),
            xml_compiler.get_appid("org.pqrs.prefix.test1111"));
  EXPECT_EQ(xml_compiler.get_symbol_map_value("ApplicationType::UNKNOWN"),
            xml_compiler.get_appid("dummy.org.pqrs.prefix.test"));

  EXPECT_EQ(xml_compiler.get_symbol_map_value("ApplicationType::SUFFIX_TEST"),
            xml_compiler.get_appid("test.suffix.pqrs.org"));
  EXPECT_EQ(xml_compiler.get_symbol_map_value("ApplicationType::SUFFIX_TEST"),
            xml_compiler.get_appid("1111test.suffix.pqrs.org"));
  EXPECT_EQ(xml_compiler.get_symbol_map_value("ApplicationType::UNKNOWN"),
            xml_compiler.get_appid("test.suffix.pqrs.org.dummy"));

  EXPECT_EQ(boost::optional<uint32_t>(0x03f0), xml_compiler.get_symbol_map_value("DeviceVendor::HEWLETT_PACKARD"));
  EXPECT_EQ(boost::optional<uint32_t>(0x9999), xml_compiler.get_symbol_map_value("DeviceVendor::SPACE_IS_IGNORED"));
  EXPECT_EQ(boost::optional<uint32_t>(0x0224), xml_compiler.get_symbol_map_value("DeviceProduct::MY_HP_KEYBOARD"));
  EXPECT_EQ(boost::optional<uint32_t>(0x9999), xml_compiler.get_symbol_map_value("DeviceProduct::SPACE_IS_IGNORED"));

  auto node_tree = xml_compiler.get_preferences_checkbox_node_tree();
  EXPECT_TRUE(node_tree.get_children());
  auto node_ptr = (*(node_tree.get_children()))[0];
  EXPECT_EQ("Swap Space and Tab\n  appendix1\n  appendix123\n  appendix123_2", node_ptr->get_node().get_name());

  {
    std::vector<uint32_t> actual;
    EXPECT_TRUE(xml_compiler.debug_get_initialize_vector(actual, "remap.showstatusmessage"));

    std::vector<uint32_t> expected;

    // <autogen>__ShowStatusMessage__ A B C D E F G -- </autogen>

    expected.push_back(17); // count
    expected.push_back(BRIDGE_STATUSMESSAGE);
    expected.push_back(65); // A
    expected.push_back(32); // space
    expected.push_back(66); // B
    expected.push_back(32); // space
    expected.push_back(67); // C
    expected.push_back(32); // space
    expected.push_back(68); // D
    expected.push_back(32); // space
    expected.push_back(69); // E
    expected.push_back(32); // space
    expected.push_back(70); // F
    expected.push_back(32); // space
    expected.push_back(71); // G
    expected.push_back(32); // space
    expected.push_back(45); // -
    expected.push_back(45); // -

    EXPECT_EQ(expected, actual);
  }

  {
    std::vector<uint32_t> actual;
    EXPECT_TRUE(xml_compiler.debug_get_initialize_vector(actual, "remap.showstatusmessage_old_style"));

    std::vector<uint32_t> expected;

    // <autogen>--ShowStatusMessage-- O L D 1 2 3 4 -- </autogen>

    expected.push_back(17); // count
    expected.push_back(BRIDGE_STATUSMESSAGE);
    expected.push_back(79); // O
    expected.push_back(32); // space
    expected.push_back(76); // L
    expected.push_back(32); // space
    expected.push_back(68); // D
    expected.push_back(32); // space
    expected.push_back(49); // 1
    expected.push_back(32); // space
    expected.push_back(50); // 2
    expected.push_back(32); // space
    expected.push_back(51); // 3
    expected.push_back(32); // space
    expected.push_back(52); // 4
    expected.push_back(32); // space
    expected.push_back(45); // -
    expected.push_back(45); // -

    EXPECT_EQ(expected, actual);
  }
}

TEST(pqrs_xml_compiler, reload_bindings_clang)
{
  pqrs_xml_compiler* p = NULL;
  EXPECT_EQ(0, pqrs_xml_compiler_initialize(&p, "data/system_xml", "data/private_xml"));
  pqrs_xml_compiler_reload(p);

  {
    uint32_t inputsource;
    uint32_t inputsource_detail;

    pqrs_xml_compiler_get_inputsourceid(p,
                                        &inputsource,
                                        &inputsource_detail,
                                        "fr",
                                        "com.apple.keylayout.French",
                                        NULL);
    EXPECT_TRUE(inputsource != 0);
    EXPECT_TRUE(inputsource_detail != 0);
  }

  {
    uint32_t inputsource;
    uint32_t inputsource_detail;

    pqrs_xml_compiler_get_inputsourceid(p,
                                        &inputsource,
                                        &inputsource_detail,
                                        NULL,
                                        NULL,
                                        NULL);
    EXPECT_TRUE(inputsource == 0);
    EXPECT_TRUE(inputsource_detail == 0);
  }

  pqrs_xml_compiler_terminate(&p);
}

TEST(pqrs_xml_compiler, reload_invalid_xml)
{
  // ------------------------------------------------------------
  // invalid XML format
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/broken_xml");
    xml_compiler.reload();
    EXPECT_EQ("<data/invalid_xml/broken_xml/private.xml>(4): expected element name",
              xml_compiler.get_error_information().get_message());
    EXPECT_EQ(boost::optional<uint32_t>(2), xml_compiler.get_symbol_map_value("ConsumerKeyCode::BRIGHTNESS_UP"));
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/broken_include");
    xml_compiler.reload();
    EXPECT_EQ("<data/invalid_xml/broken_include/include.xml>(4): expected element name",
              xml_compiler.get_error_information().get_message());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/missing_include");
    xml_compiler.reload();
    EXPECT_EQ("data/invalid_xml/missing_include/include.xml is not found.",
              xml_compiler.get_error_information().get_message());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/infinite_include_loop");
    xml_compiler.reload();
    EXPECT_EQ("An infinite include loop is detected:\n"
              "data/invalid_xml/infinite_include_loop/private.xml",
              xml_compiler.get_error_information().get_message());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/infinite_include_loop2");
    xml_compiler.reload();
    EXPECT_EQ("An infinite include loop is detected:\n"
              "data/invalid_xml/infinite_include_loop2/include.xml",
              xml_compiler.get_error_information().get_message());
  }

  // ------------------------------------------------------------
  // identifier
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/dup_identifier");
    xml_compiler.reload();
    const char* message = "Duplicated identifier:\n"
                          "\n"
                          "<identifier>private.swap_space_and_tab</identifier>";
    EXPECT_EQ(message, xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/empty_identifier");
    xml_compiler.reload();
    EXPECT_EQ("Empty <identifier>.", xml_compiler.get_error_information().get_message());
    EXPECT_EQ(2, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/empty_identifier2");
    xml_compiler.reload();
    EXPECT_EQ("Empty <identifier>.", xml_compiler.get_error_information().get_message());
    EXPECT_EQ(2, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/invalid_identifier_place");
    xml_compiler.reload();
    const char* message = "<identifier> must be placed directly under <item>:\n"
                          "\n"
                          "<identifier>private.swap_space_and_tab</identifier>";
    EXPECT_EQ(message, xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/invalid_identifier_place2");
    xml_compiler.reload();
    const char* message = "<identifier> must be placed directly under <item>:\n"
                          "\n"
                          "<identifier>private.swap_space_and_tab</identifier>";
    EXPECT_EQ(message, xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }

  // ------------------------------------------------------------
  // autogen
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/unknown_autogen");
    xml_compiler.reload();
    const char* message = "Invalid <autogen>:\n"
                          "\n"
                          "<autogen>__KeyToKey2__ KeyCode::SPACE, VK_SHIFT, KeyCode::TAB</autogen>";
    EXPECT_EQ(message, xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());

    {
      std::vector<uint32_t> actual;
      EXPECT_TRUE(xml_compiler.debug_get_initialize_vector(actual, "private.swap_space_and_tab"));

      // empty if error
      std::vector<uint32_t> expected;
      EXPECT_EQ(expected, actual);
    }
    {
      std::vector<uint32_t> actual;
      EXPECT_TRUE(xml_compiler.debug_get_initialize_vector(actual, "private.swap_space_and_tab2"));

      std::vector<uint32_t> expected;

      int config_index_notsave_passthrough = 2;

      // ------------------------------------------------------------
      // <autogen>__KeyToKey__ KeyCode::SPACE, VK_SHIFT, KeyCode::TAB</autogen>
      expected.push_back(7);      // count
      expected.push_back(BRIDGE_REMAPTYPE_KEYTOKEY);
      expected.push_back(BRIDGE_DATATYPE_KEYCODE);
      expected.push_back(49);     // KeyCode::SPACE
      expected.push_back(BRIDGE_DATATYPE_FLAGS);
      expected.push_back(131074); // ModifierFlag::SHIFT_L
      expected.push_back(BRIDGE_DATATYPE_KEYCODE);
      expected.push_back(48);     // KeyCode::TAB

      expected.push_back(2);      // count
      expected.push_back(BRIDGE_FILTERTYPE_CONFIG_NOT);
      expected.push_back(config_index_notsave_passthrough); // ConfigIndex::notsave_passthrough

      expected.push_back(7);      // count
      expected.push_back(BRIDGE_REMAPTYPE_KEYTOKEY);
      expected.push_back(BRIDGE_DATATYPE_KEYCODE);
      expected.push_back(49);     // KeyCode::SPACE
      expected.push_back(BRIDGE_DATATYPE_FLAGS);
      expected.push_back(131076); // ModifierFlag::SHIFT_R
      expected.push_back(BRIDGE_DATATYPE_KEYCODE);
      expected.push_back(48);     // KeyCode::TAB

      expected.push_back(2);      // count
      expected.push_back(BRIDGE_FILTERTYPE_CONFIG_NOT);
      expected.push_back(config_index_notsave_passthrough); // ConfigIndex::notsave_passthrough

      // ------------------------------------------------------------
      // <autogen>--KeyToKey-- KeyCode::TAB, VK_SHIFT, KeyCode::SPACE</autogen>
      expected.push_back(7);      // count
      expected.push_back(BRIDGE_REMAPTYPE_KEYTOKEY);
      expected.push_back(BRIDGE_DATATYPE_KEYCODE);
      expected.push_back(48);     // KeyCode::TAB
      expected.push_back(BRIDGE_DATATYPE_FLAGS);
      expected.push_back(131074); // ModifierFlag::SHIFT_L
      expected.push_back(BRIDGE_DATATYPE_KEYCODE);
      expected.push_back(49);     // KeyCode::SPACE

      expected.push_back(2);      // count
      expected.push_back(BRIDGE_FILTERTYPE_CONFIG_NOT);
      expected.push_back(config_index_notsave_passthrough); // ConfigIndex::notsave_passthrough

      expected.push_back(7);      // count
      expected.push_back(BRIDGE_REMAPTYPE_KEYTOKEY);
      expected.push_back(BRIDGE_DATATYPE_KEYCODE);
      expected.push_back(48);     // KeyCode::TAB
      expected.push_back(BRIDGE_DATATYPE_FLAGS);
      expected.push_back(131076); // ModifierFlag::SHIFT_R
      expected.push_back(BRIDGE_DATATYPE_KEYCODE);
      expected.push_back(49);     // KeyCode::SPACE

      expected.push_back(2);      // count
      expected.push_back(BRIDGE_FILTERTYPE_CONFIG_NOT);
      expected.push_back(config_index_notsave_passthrough); // ConfigIndex::notsave_passthrough

      EXPECT_EQ(expected, actual);
    }
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/autogen_invalid_pipe_type");
    xml_compiler.reload();
    const char* message = "Cannot connect(|) except ModifierFlag and PointingButton:\n"
                          "\n"
                          "KeyCode::SPACE|KeyCode::TAB";
    EXPECT_EQ(message, xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/autogen_invalid_pipe_different_type");
    xml_compiler.reload();
    const char* message = "Cannot connect(|) between different types:\n"
                          "\n"
                          "ModifierFlag::SHIFT_L|PointingButton::LEFT";
    EXPECT_EQ(message, xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }

  // ------------------------------------------------------------
  // Unknown symbol_map
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/unknown_symbol_map");
    xml_compiler.reload();
    EXPECT_EQ("Unknown symbol:\n\nKeyCode::MY_UNKNOWN_KEY", xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }

  // ------------------------------------------------------------
  // Unknown data_type
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/unknown_data_type");
    xml_compiler.reload();
    EXPECT_EQ("Unknown symbol:\n\nKeyCode2::SPACE", xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }

  // ------------------------------------------------------------
  // replacementdef.xml
  {
    pqrs::xml_compiler xml_compiler("data/invalid_xml/replacementdef_no_name", "data/private_xml");
    xml_compiler.reload();
    EXPECT_EQ("No <replacementname> within <replacementdef>.", xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/invalid_xml/replacementdef_empty_name", "data/private_xml");
    xml_compiler.reload();
    EXPECT_EQ("Empty <replacementname> within <replacementdef>.", xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/invalid_xml/replacementdef_invalid_name1", "data/private_xml");
    xml_compiler.reload();
    EXPECT_EQ("Do not use '{{' and '}}' within <replacementname>:\n\nVI{{_J", xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/invalid_xml/replacementdef_invalid_name2", "data/private_xml");
    xml_compiler.reload();
    EXPECT_EQ("Do not use '{{' and '}}' within <replacementname>:\n\nVI_}}J", xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/invalid_xml/replacementdef_no_value", "data/private_xml");
    xml_compiler.reload();
    EXPECT_EQ("No <replacementvalue> within <replacementdef>:\n\nVI_J", xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }

  // ------------------------------------------------------------
  // symbol_map.xml
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/symbol_map_xml_no_type");
    xml_compiler.reload();
    EXPECT_EQ("No 'type' Attribute within <symbol_map>.", xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/symbol_map_xml_empty_type");
    xml_compiler.reload();
    EXPECT_EQ("Empty 'type' Attribute within <symbol_map>.", xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/symbol_map_xml_no_name");
    xml_compiler.reload();
    EXPECT_EQ("No 'name' Attribute within <symbol_map>.", xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/symbol_map_xml_empty_name");
    xml_compiler.reload();
    EXPECT_EQ("Empty 'name' Attribute within <symbol_map>.", xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/symbol_map_xml_no_value");
    xml_compiler.reload();
    EXPECT_EQ("No 'value' Attribute within <symbol_map>.", xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/symbol_map_xml_empty_value");
    xml_compiler.reload();
    EXPECT_EQ("Empty 'value' Attribute within <symbol_map>.", xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/symbol_map_xml_invalid_value");
    xml_compiler.reload();
    const char* message = "Invalid 'value' Attribute within <symbol_map>:\n"
                          "\n"
                          "<symbol_map type=\"ConsumerKeyCode\" name=\"BRIGHTNESS_UP\" value=\"XXX\" />";
    EXPECT_EQ(message, xml_compiler.get_error_information().get_message());
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }

  // ------------------------------------------------------------
  // appdef.xml
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/appdef_no_name");
    xml_compiler.reload();
    EXPECT_EQ("No <appname> within <appdef>.", std::string(xml_compiler.get_error_information().get_message()));
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/appdef_empty_name");
    xml_compiler.reload();
    EXPECT_EQ("Empty <appname> within <appdef>.", std::string(xml_compiler.get_error_information().get_message()));
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }

  // ------------------------------------------------------------
  // devicevendordef.xml
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/devicevendordef_no_name");
    xml_compiler.reload();
    EXPECT_EQ("No <vendorname> within <devicevendordef>.", std::string(xml_compiler.get_error_information().get_message()));
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/devicevendordef_empty_name");
    xml_compiler.reload();
    EXPECT_EQ("Empty <vendorname> within <devicevendordef>.", std::string(xml_compiler.get_error_information().get_message()));
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/devicevendordef_no_value");
    xml_compiler.reload();
    EXPECT_EQ("No <vendorid> within <devicevendordef>.", std::string(xml_compiler.get_error_information().get_message()));
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/devicevendordef_empty_value");
    xml_compiler.reload();
    EXPECT_EQ("Empty <vendorid> within <devicevendordef>.", std::string(xml_compiler.get_error_information().get_message()));
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/devicevendordef_invalid_value");
    xml_compiler.reload();
    const char* message = "Invalid <vendorid> within <devicevendordef>:\n"
                          "\n"
                          "<vendorid>XXX</vendorid>";
    EXPECT_EQ(message, std::string(xml_compiler.get_error_information().get_message()));
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }

  // ------------------------------------------------------------
  // deviceproductdef.xml
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/deviceproductdef_no_name");
    xml_compiler.reload();
    EXPECT_EQ("No <productname> within <deviceproductdef>.", std::string(xml_compiler.get_error_information().get_message()));
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/deviceproductdef_empty_name");
    xml_compiler.reload();
    EXPECT_EQ("Empty <productname> within <deviceproductdef>.", std::string(xml_compiler.get_error_information().get_message()));
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/deviceproductdef_no_value");
    xml_compiler.reload();
    EXPECT_EQ("No <productid> within <deviceproductdef>.", std::string(xml_compiler.get_error_information().get_message()));
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/deviceproductdef_empty_value");
    xml_compiler.reload();
    EXPECT_EQ("Empty <productid> within <deviceproductdef>.", std::string(xml_compiler.get_error_information().get_message()));
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/deviceproductdef_invalid_value");
    xml_compiler.reload();
    const char* message = "Invalid <productid> within <deviceproductdef>:\n"
                          "\n"
                          "<productid>XXX</productid>";
    EXPECT_EQ(message, std::string(xml_compiler.get_error_information().get_message()));
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }

  // ------------------------------------------------------------
  // inputsourcedef.xml
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/vkchangeinputsourcedef_no_name");
    xml_compiler.reload();
    EXPECT_EQ("No <name> within <vkchangeinputsourcedef>.", std::string(xml_compiler.get_error_information().get_message()));
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/vkchangeinputsourcedef_empty_name");
    xml_compiler.reload();
    EXPECT_EQ("<name> within <vkchangeinputsourcedef> must start with \"KeyCode::VK_CHANGE_INPUTSOURCE_\":\n"
              "\n"
              "<name></name>",
              std::string(xml_compiler.get_error_information().get_message()));
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }

  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/inputsourcedef_no_name");
    xml_compiler.reload();
    EXPECT_EQ("No <name> within <inputsourcedef>.", std::string(xml_compiler.get_error_information().get_message()));
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
  {
    pqrs::xml_compiler xml_compiler("data/system_xml", "data/invalid_xml/inputsourcedef_empty_name");
    xml_compiler.reload();
    EXPECT_EQ("Empty <name> within <inputsourcedef>.",
              std::string(xml_compiler.get_error_information().get_message()));
    EXPECT_EQ(1, xml_compiler.get_error_information().get_count());
  }
}

TEST(pqrs_xml_compiler_symbol_map, add)
{
  pqrs::xml_compiler::symbol_map s;
  s.add("KeyCode", "SPACE", 36);
  s.add("KeyCode", "VK__AUTOINDEX__BEGIN__", 1024);
  s.add("KeyCode", "VK_NEW1");
  s.add("KeyCode", "VK_NEW2");

  EXPECT_EQ(static_cast<uint32_t>(1024), s.get("KeyCode::VK_NEW1"));
  EXPECT_EQ(static_cast<uint32_t>(1025), s.get("KeyCode::VK_NEW2"));
}

TEST(pqrs_xml_compiler_remapclasses_initialize_vector, get)
{
  pqrs::xml_compiler::remapclasses_initialize_vector v;
  EXPECT_EQ(2, v.get().size());
}

TEST(pqrs_xml_compiler_remapclasses_initialize_vector, add)
{
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
  expected.push_back(BRIDGE_REMAPCLASS_INITIALIZE_VECTOR_FORMAT_VERSION);
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

  EXPECT_EQ(expected, v.get());
}

TEST(pqrs_xml_compiler_remapclasses_initialize_vector, add_partial)
{
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
  expected.push_back(BRIDGE_REMAPCLASS_INITIALIZE_VECTOR_FORMAT_VERSION);
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

  EXPECT_EQ(expected, v.get());
}

TEST(pqrs_xml_compiler_filter_vector, filter_vector)
{
  pqrs::xml_compiler::symbol_map s;
  s.add("ApplicationType", "APP1", 1);
  s.add("ApplicationType", "APP2", 2);
  s.add("ApplicationType", "APP3", 3);
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
  s.add("InputSourceDetail", "NONE", 0);
  s.add("InputSourceDetail", "INPUTSOURCEDETAIL1", 120);
  s.add("InputSourceDetail", "INPUTSOURCEDETAIL2", 121);
  s.add("InputSourceDetail", "INPUTSOURCEDETAIL3", 122);
  s.add("ConfigIndex", "config1", 1000);
  s.add("ConfigIndex", "config2", 2000);
  s.add("ConfigIndex", "config3", 3000);
  s.add("ModifierFlag", "MOD1", 0x1000);
  s.add("ModifierFlag", "MOD2", 0x2000);
  s.add("ModifierFlag", "MOD3", 0x4000);

  std::string xml("<?xml version=\"1.0\"?>"
                  "<item>"
                  "  <only>APP1,APP3</only>"
                  "  <only><!-- empty --></only>"
                  "  <only>,,</only>"
                  "  <not><!-- XXX --->APP2</not>"
                  "  <identifier>sample</identifier>"
                  "  <device_only>DeviceVendor::VENDOR1, DeviceProduct::PRODUCT1, DeviceLocation::LOCATION1</device_only>"
                  "  <device_not>"
                  "    DeviceVendor::VENDOR3,,,,"
                  "    DeviceProduct::PRODUCT3,"
                  "    DeviceVendor::VENDOR3,"
                  "  </device_not>"
                  "  <device_not></device_not>"
                  "  <inputsource_only>INPUTSOURCE1</inputsource_only>"
                  "  <inputsource_not>INPUTSOURCE2, INPUTSOURCE3</inputsource_not>"
                  "  <inputmode_only>INPUTSOURCE1</inputmode_only>"
                  "  <inputmode_not>INPUTSOURCE2, INPUTSOURCE3</inputmode_not>"
                  "  <inputsourcedetail_only>INPUTSOURCEDETAIL1</inputsourcedetail_only>"
                  "  <inputsourcedetail_not>INPUTSOURCEDETAIL2, INPUTSOURCEDETAIL3</inputsourcedetail_not>"
                  "  <inputmodedetail_only>INPUTSOURCEDETAIL1</inputmodedetail_only>"
                  "  <inputmodedetail_not>INPUTSOURCEDETAIL2, INPUTSOURCEDETAIL3</inputmodedetail_not>"
                  "  <config_only>config1,config2</config_only>"
                  "  <config_not>config3</config_not>"
                  "  <modifier_only>ModifierFlag::MOD1 ||| ModifierFlag::MOD3</modifier_only>"
                  "  <modifier_not> ModifierFlag::MOD2 </modifier_not>"
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

    // <inputsource_only>InputSource::INPUTSOURCE1</inputsource_only>
    expected.push_back(2); // count
    expected.push_back(BRIDGE_FILTERTYPE_INPUTSOURCE_ONLY);
    expected.push_back(110);

    // <inputsource_not>InputSource::INPUTSOURCE2, InputSource::INPUTSOURCE3</inputsource_not>
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

    // <inputsourcedetail_only>INPUTSOURCEDETAIL1</inputsourcedetail_only>
    expected.push_back(2); // count
    expected.push_back(BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_ONLY);
    expected.push_back(120);

    // <inputsourcedetail_not>INPUTSOURCEDETAIL2, INPUTSOURCEDETAIL3</inputsourcedetail_not>
    expected.push_back(3); // count
    expected.push_back(BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_NOT);
    expected.push_back(121);
    expected.push_back(122);

    // <inputmodedetail_only>INPUTSOURCEDETAIL1</inputmodedetail_only>
    expected.push_back(2); // count
    expected.push_back(BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_ONLY);
    expected.push_back(120);

    // <inputmodedetail_not>INPUTSOURCEDETAIL2, INPUTSOURCEDETAIL3</inputmodedetail_not>
    expected.push_back(3); // count
    expected.push_back(BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_NOT);
    expected.push_back(121);
    expected.push_back(122);

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
    expected.push_back(2);
    expected.push_back(BRIDGE_FILTERTYPE_MODIFIER_ONLY);
    expected.push_back(0x1000 | 0x4000);

    // <modifier_not> ModifierFlag::MOD2 </modifier_not>
    expected.push_back(2);
    expected.push_back(BRIDGE_FILTERTYPE_MODIFIER_NOT);
    expected.push_back(0x2000);

    EXPECT_EQ(expected, fv.get());
  }
}

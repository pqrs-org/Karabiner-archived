#!/usr/bin/ruby

require 'rubygems'
require 'xml/libxml'

require 'inc.filter.rb'
require 'inc.remapclass.rb'
require 'inc.keycode.rb'

GC.disable

$outfile = {
  :config => open('output/include.config.hpp', 'w'),
  :config_SYSCTL => open('output/include.config_SYSCTL.cpp', 'w'),
  :config_register => open('output/include.config_register.cpp', 'w'),
  :config_unregister => open('output/include.config_unregister.cpp', 'w'),
  :remapclass_initialize_vector => open('output/include.RemapClass_initialize_vector.cpp', 'w'),
}

# ======================================================================
KeyCode.load_keycode

ARGV.each do |xmlpath|
  parser = XML::Parser.file(xmlpath)
  libxmldoc = parser.parse

  # ------------------------------------------------------------
  # setup KeyCode
  KeyCode.setup_configindex(libxmldoc.root)

  libxmldoc.root.find('//vk_config').each do |node|
    list = node.parent.find('./sysctl')
    throw :exit if list.length != 1
    name = list[0].inner_xml.gsub(/\./, '_')

    KeyCode.append_autoindexed('KeyCode', "VK_CONFIG_TOGGLE_#{name}")
    KeyCode.append_autoindexed('KeyCode', "VK_CONFIG_FORCE_ON_#{name}")
    KeyCode.append_autoindexed('KeyCode', "VK_CONFIG_FORCE_OFF_#{name}")
    KeyCode.append_autoindexed('KeyCode', "VK_CONFIG_SYNC_KEYDOWNUP_#{name}")
  end

  # ------------------------------------------------------------
  counter = 0

  libxmldoc.root.find('//item').each do |node|
    counter += 1
    if counter > 10 then
      $stdout.print '.'
      $stdout.flush
      counter = 0
    end

    # ------------------------------------------------------------
    # validate
    if node.find('./name').length != 1 then
      print "%%% ERROR no <name> or multiple <name> for #{node}\n"
      exit 1
    end

    # ------------------------------------------------------------
    sysctl_node = node.find_first('./sysctl')
    next if sysctl_node.nil?

    name = sysctl_node.inner_xml
    sysctl_entry = name.split('.')[0]
    sysctl_name  = name.split('.')[1]
    name.gsub!(/\./, '_')

    configaddress = nil
    essential = false
    if sysctl_node['essential'] == 'true' then
      configaddress = "&(Config::essential_config[BRIDGE_ESSENTIAL_CONFIG_INDEX_#{name}])"
      essential = true
    else
      configaddress = "&(Config::enabled_flags[#{KeyCode.ConfigIndex(name)}])"
    end
    $outfile[:config_SYSCTL] << "SYSCTL_PROC(_keyremap4macbook_#{sysctl_entry}, OID_AUTO, #{sysctl_name}, CTLTYPE_INT|CTLFLAG_RW, #{configaddress}, 0, refresh_remapfunc_handler, \"I\", \"\");\n"
    $outfile[:config_register] << "sysctl_register_oid(&sysctl__keyremap4macbook_#{name});\n"
    $outfile[:config_unregister] << "sysctl_unregister_oid(&sysctl__keyremap4macbook_#{name});\n"

    # ----------------------------------------
    unless essential then
      RemapClass.reset_variable_index
      remapclass = RemapClass.new(name)
      remapclass.to_code(node, $outfile[:remapclass_initialize_vector])
    end
  end
end

print "\n"

RemapClass.output_entries($outfile[:remapclass_initialize_vector])
$outfile[:config] << "static int enabled_flags[#{KeyCode.count('ConfigIndex')}];\n"

# ======================================================================
$outfile.each do |name,file|
  file.close
end

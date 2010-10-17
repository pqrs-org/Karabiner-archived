#!/usr/bin/ruby

require 'rubygems'
require 'xml/libxml'

require 'inc.preprocess.rb'
require 'inc.filter.rb'
require 'inc.remapclass.rb'
require 'inc.keycode.rb'

$outfile = {
  :config => open('output/include.config.hpp', 'w'),
  :config_SYSCTL => open('output/include.config_SYSCTL.cpp', 'w'),
  :config_register => open('output/include.config_register.cpp', 'w'),
  :config_unregister => open('output/include.config_unregister.cpp', 'w'),
  :config_default => open('output/include.config.default.hpp', 'w'),
  :remapclass_initialize_vector => open('output/include.RemapClass_initialize_vector.cpp', 'w'),
  :remapcode_vk_config => open('output/include.remapcode_vk_config.cpp', 'w'),
}

# ======================================================================
ARGV.each do |xmlpath|
  lines = Preprocesser.new().preprocess(IO.readlines(xmlpath))
  parser = XML::Parser.string(lines.join(''))
  libxmldoc = parser.parse

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
      configaddress = "&(config.#{name})"
      essential = true
      $outfile[:config] << "int #{name};\n"
    else
      configaddress = "&(config.enabled_flags[#{KeyCode.ConfigIndex(name)}])"
    end
    $outfile[:config_SYSCTL] << "SYSCTL_PROC(_keyremap4macbook_#{sysctl_entry}, OID_AUTO, #{sysctl_name}, CTLTYPE_INT|CTLFLAG_RW, #{configaddress}, 0, refresh_remapfunc_handler, \"I\", \"\");\n"
    $outfile[:config_register] << "sysctl_register_oid(&sysctl__keyremap4macbook_#{name});\n"
    $outfile[:config_unregister] << "sysctl_unregister_oid(&sysctl__keyremap4macbook_#{name});\n"

    # ----------------------------------------
    default_node = node.find_first('./default')
    unless default_node.nil? then
      $outfile[:config_default] << "#{name} = #{default_node.inner_xml};\n"
    end

    # ----------------------------------------
    if node.find_first('./vk_config') then
      $outfile[:remapcode_vk_config] << "if (params.key == KeyCode::VK_CONFIG_TOGGLE_#{name}) {\n"
      $outfile[:remapcode_vk_config] << "  configitem = #{configaddress};\n"
      $outfile[:remapcode_vk_config] << "  type = TYPE_TOGGLE;\n"
      $outfile[:remapcode_vk_config] << "}\n"
      $outfile[:remapcode_vk_config] << "if (params.key == KeyCode::VK_CONFIG_FORCE_ON_#{name}) {\n"
      $outfile[:remapcode_vk_config] << "  configitem = #{configaddress};\n"
      $outfile[:remapcode_vk_config] << "  type = TYPE_FORCE_ON;\n"
      $outfile[:remapcode_vk_config] << "}\n"
      $outfile[:remapcode_vk_config] << "if (params.key == KeyCode::VK_CONFIG_FORCE_OFF_#{name}) {\n"
      $outfile[:remapcode_vk_config] << "  configitem = #{configaddress};\n"
      $outfile[:remapcode_vk_config] << "  type = TYPE_FORCE_OFF;\n"
      $outfile[:remapcode_vk_config] << "}\n"
      $outfile[:remapcode_vk_config] << "if (params.key == KeyCode::VK_CONFIG_SYNC_KEYDOWNUP_#{name}) {\n"
      $outfile[:remapcode_vk_config] << "  configitem = #{configaddress};\n"
      $outfile[:remapcode_vk_config] << "  type = TYPE_SYNC_KEYDOWNUP;\n"
      $outfile[:remapcode_vk_config] << "}\n"
    end

    # ----------------------------------------
    unless essential then
      RemapClass.reset_variable_index
      remapclass = RemapClass.new(name)
      remapclass.to_code(node, $outfile[:remapclass_initialize_vector])
    end
  end
end

print "\n"

$outfile[:config] << "int enabled_flags[#{KeyCode.count('ConfigIndex')}];\n"

# ======================================================================
$outfile.each do |name,file|
  file.close
end

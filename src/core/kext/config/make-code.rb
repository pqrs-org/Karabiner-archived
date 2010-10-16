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
  :remapclass => open('output/include.RemapClass.cpp', 'w'),
  :remapcode_vk_config => open('output/include.remapcode_vk_config.cpp', 'w'),
}

# ======================================================================
def parseautogen(name, lines)
  remapclass = RemapClass.new(name)

  while true
    l = lines.shift
    break if l.nil?

    # --------------------------------------------------
    next if /<\/?item>/ =~ l
    next if /<name>.+?<\/name>/ =~ l
    next if /<appendix>.+?<\/appendix>/ =~ l
    next if /<sysctl.*?>.+?<\/sysctl>/ =~ l
    next if /<baseunit>.+?<\/baseunit>/ =~ l
    next if /<default>.+?<\/default>/ =~ l
    next if /<vk_config>.+?<\/vk_config>/ =~ l

    # --------------------------------------------------
    if /<block>/ =~ l then
      block_remapclass = parseautogen(name, lines)

      remapclass += block_remapclass

    elsif /<\/block>/ =~ l then
      break

    elsif remapclass.parse(l) then
      # do nothing

    elsif /<.+?>.+?<\/.+?>/ =~ l then
      print "%%% ERROR unknown command %%%\n#{l}\n"
      exit 1
    end
  end

  # ======================================================================
  remapclass.fixup
  return remapclass
end

# ----------------------------------------------------------------------
ARGV.each do |xmlpath|
  lines = Preprocesser.new().preprocess(IO.readlines(xmlpath))
  parser = XML::Parser.string(lines.join(''))
  libxmldoc = parser.parse

  libxmldoc.root.find('//item').each do |node|
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
    if sysctl_node['essential'] == 'true' then
      configaddress = "&(config.#{name})"
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
    RemapClass.reset_variable_index
    lines = node.to_s.split(/\n/)
    remapclass = parseautogen(name, lines)
    next if remapclass.empty?
    $outfile[:remapclass] << remapclass.to_code
  end
end

$outfile[:config] << "int enabled_flags[#{KeyCode.count('ConfigIndex')}];\n"

# ======================================================================
# put all entries
[:initialize,
 :terminate,
 :remap_setkeyboardtype,
 :remap_key,
 :remap_consumer,
 :remap_pointing,
 :remap_simultaneouskeypresses,
 :remap_dropkeyafterremap,
 :get_statusmessage,
 :enabled,
].each do |key|
  name = key.to_s
  $outfile[:remapclass] << "RemapClass_#{name} listRemapClass_#{name}[] = {\n"

  [true, false].each do |isnotsave|
    RemapClass.get_entries.each do |item|
      next if isnotsave && ! (/^notsave_/ =~ item[:name])
      next if ! isnotsave && (/^notsave_/ =~ item[:name])

      item[key].each do |line|
        $outfile[:remapclass] << "#{line},\n"
      end
    end
  end

  $outfile[:remapclass] << "NULL,\n"
  $outfile[:remapclass] << "};\n"
end

# ======================================================================
$outfile.each do |name,file|
  file.close
end

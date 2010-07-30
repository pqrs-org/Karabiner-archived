#!/usr/bin/ruby

require 'inc.preprocess.rb'
require 'inc.filter.rb'
require 'inc.remapclass.rb'

$outfile = {
  :config => open('output/include.config.hpp', 'w'),
  :config_SYSCTL => open('output/include.config_SYSCTL.cpp', 'w'),
  :config_register => open('output/include.config_register.cpp', 'w'),
  :config_unregister => open('output/include.config_unregister.cpp', 'w'),
  :config_default => open('output/include.config.default.hpp', 'w'),
  :remapclass => open('output/include.RemapClass.cpp', 'w'),
  :remapcode_vk_config => open('output/include.remapcode_vk_config.cpp', 'w'),
  :keycode_vk_config => open('../keycode/data/include/KeyCode.VK_GENERATED.data', 'w'),
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
    next if /<sysctl>.+?<\/sysctl>/ =~ l
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
$stdin.read.scan(/<item>.+?<\/item>/m).each do |item|
  if /.*(<item>.+?<\/item>)/m =~ item then
    item = $1
  end

  name = nil

  if /<sysctl>([^\.]+?)\.(.+?)<\/sysctl>/m =~ item then
    name = "#{$1}_#{$2}"
    $outfile[:config_SYSCTL] << "SYSCTL_PROC(_keyremap4macbook_#{$1}, OID_AUTO, #{$2}, CTLTYPE_INT|CTLFLAG_RW, &(config.#{name}), 0, refresh_remapfunc_handler, \"I\", \"\");\n"
    $outfile[:config_register] << "sysctl_register_oid(&sysctl__keyremap4macbook_#{name});\n"
    $outfile[:config_unregister] << "sysctl_unregister_oid(&sysctl__keyremap4macbook_#{name});\n"
  end

  next if name.nil?

  $outfile[:config] << "int #{name};\n"

  if /<default>(.+?)<\/default>/m =~ item then
    $outfile[:config_default] << "#{name} = #{$1};\n"
  end

  # check <name> num == 1
  if item.scan(/<name>(.+?)<\/name>/m).size != 1 then
    print "%%% ERROR no <name> or multiple <name> for #{name}\n"
    exit 1
  end

  if /<vk_config>true<\/vk_config>/ =~ item then
    $outfile[:remapcode_vk_config] << "if (params.key == KeyCode::VK_CONFIG_TOGGLE_#{name}) {\n"
    $outfile[:remapcode_vk_config] << "  configitem = &(config.#{name});\n"
    $outfile[:remapcode_vk_config] << "  type = TYPE_TOGGLE;\n"
    $outfile[:remapcode_vk_config] << "}\n"
    $outfile[:remapcode_vk_config] << "if (params.key == KeyCode::VK_CONFIG_FORCE_ON_#{name}) {\n"
    $outfile[:remapcode_vk_config] << "  configitem = &(config.#{name});\n"
    $outfile[:remapcode_vk_config] << "  type = TYPE_FORCE_ON;\n"
    $outfile[:remapcode_vk_config] << "}\n"
    $outfile[:remapcode_vk_config] << "if (params.key == KeyCode::VK_CONFIG_FORCE_OFF_#{name}) {\n"
    $outfile[:remapcode_vk_config] << "  configitem = &(config.#{name});\n"
    $outfile[:remapcode_vk_config] << "  type = TYPE_FORCE_OFF;\n"
    $outfile[:remapcode_vk_config] << "}\n"
    $outfile[:remapcode_vk_config] << "if (params.key == KeyCode::VK_CONFIG_SYNC_KEYDOWNUP_#{name}) {\n"
    $outfile[:remapcode_vk_config] << "  configitem = &(config.#{name});\n"
    $outfile[:remapcode_vk_config] << "  type = TYPE_SYNC_KEYDOWNUP;\n"
    $outfile[:remapcode_vk_config] << "}\n"

    $outfile[:keycode_vk_config] << "VK_CONFIG_TOGGLE_#{name} --AUTO--\n"
    $outfile[:keycode_vk_config] << "VK_CONFIG_FORCE_ON_#{name} --AUTO--\n"
    $outfile[:keycode_vk_config] << "VK_CONFIG_FORCE_OFF_#{name} --AUTO--\n"
    $outfile[:keycode_vk_config] << "VK_CONFIG_SYNC_KEYDOWNUP_#{name} --AUTO--\n"
  end

  # ======================================================================
  lines = item.split(/\n/)
  lines = Preprocesser.new().preprocess(lines)

  remapclass = parseautogen(name, lines)

  unless lines.empty? then
    print "%%% ERROR no </block> at #{name} %%%\n"
  end

  next if remapclass.empty?

  $outfile[:keycode_vk_config] << remapclass.code[:keycode]
  $outfile[:remapclass] << remapclass.to_code
end

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

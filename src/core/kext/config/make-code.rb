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
  :remapcode_keyboardtype => open('output/include.remapcode_keyboardtype.hpp', 'w'),
  :remapcode_func => open('output/include.remapcode_func.cpp', 'w'),
  :remapcode_info => open('output/include.remapcode_info.cpp', 'w'),
  :remapcode_refresh_remapfunc_key => open('output/include.remapcode_refresh_remapfunc_key.cpp', 'w'),
  :remapcode_refresh_remapfunc_consumer => open('output/include.remapcode_refresh_remapfunc_consumer.cpp', 'w'),
  :remapcode_refresh_remapfunc_pointing => open('output/include.remapcode_refresh_remapfunc_pointing.cpp', 'w'),
  :remapcode_refresh_remapfunc_statusmessage => open('output/include.remapcode_refresh_remapfunc_statusmessage.cpp', 'w'),
  :remapcode_vk_config => open('output/include.remapcode_vk_config.cpp', 'w'),
  :remapcode_simultaneouskeypresses_func => open('output/include.remapcode_simultaneouskeypresses_func.cpp', 'w'),
  :keycode_vk_config => open('../keycode/data/include/KeyCode.VK_GENERATED.data', 'w'),
}

$func = {
  :key => [],
  :consumer => [],
  :pointing => [],

  :simultaneouskeypresses => [],
}

# ======================================================================
def parseautogen(name, lines, autogen_index)
  remapclass = RemapClass.new(name, autogen_index)
  code_consumer = []
  code_pointing = []
  code_variable = []

  simultaneouskeypresses_code = []
  simultaneouskeypresses_variable = []

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
      block_remapclass = parseautogen(name, lines, remapclass.autogen_index)

      remapclass += block_remapclass
      remapclass.autogen_index = block_remapclass.autogen_index

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

    $outfile[:keycode_vk_config] << "VK_CONFIG_TOGGLE_#{name} --AUTO--\n"
    $outfile[:keycode_vk_config] << "VK_CONFIG_FORCE_ON_#{name} --AUTO--\n"
    $outfile[:keycode_vk_config] << "VK_CONFIG_FORCE_OFF_#{name} --AUTO--\n"
  end

  # ======================================================================
  lines = item.split(/\n/)
  lines = Preprocesser.new().preprocess(lines)

  remapclass = parseautogen(name, lines, 0)

  unless lines.empty? then
    print "%%% ERROR no </block> at #{name} %%%\n"
  end

  $outfile[:remapcode_keyboardtype] << remapclass.code[:remap_setkeyboardtype]
  $outfile[:remapcode_refresh_remapfunc_statusmessage] << remapclass.code[:refresh_remapfunc_statusmessage]
  $outfile[:keycode_vk_config] << remapclass.code[:keycode]

  next if remapclass.empty?

  $outfile[:remapcode_func] << remapclass.to_code

  unless remapclass.code[:simultaneouskeypresses_variable].empty? then
    $outfile[:remapcode_simultaneouskeypresses_func] << "class RemapClass_#{name} : public KeyEventInputQueue::RemapClass {\n"
    $outfile[:remapcode_simultaneouskeypresses_func] << "public:\n"

    $outfile[:remapcode_simultaneouskeypresses_func] << "void initialize(void) {\n"
    remapclass.code[:simultaneouskeypresses_variable].each do |variable|
      $outfile[:remapcode_simultaneouskeypresses_func] << "  #{variable[:name]}.initialize(#{variable[:params]});\n"
    end
    $outfile[:remapcode_simultaneouskeypresses_func] << "}\n"

    $outfile[:remapcode_simultaneouskeypresses_func] << "void remap(void) {\n"
    $outfile[:remapcode_simultaneouskeypresses_func] << remapclass.code[:remap_simultaneouskeypresses]
    $outfile[:remapcode_simultaneouskeypresses_func] << "}\n"

    $outfile[:remapcode_simultaneouskeypresses_func] << "bool handleVirtualKey(const Params_KeyboardEventCallBack& params) {\n"
    remapclass.code[:simultaneouskeypresses_variable].each do |variable|
      $outfile[:remapcode_simultaneouskeypresses_func] << "  if (#{variable[:name]}.handleVirtualKey(params)) return true;\n"
    end
    $outfile[:remapcode_simultaneouskeypresses_func] << "  return false;\n"
    $outfile[:remapcode_simultaneouskeypresses_func] << "}\n"

    if /^passthrough_/ =~ name then
      $outfile[:remapcode_simultaneouskeypresses_func] << "bool enabled(void) const { return config.#{name}; }\n"
    else
      $outfile[:remapcode_simultaneouskeypresses_func] << "bool enabled(void) const { return config.#{name} && ! config.notsave_passthrough; }\n"
    end

    $outfile[:remapcode_simultaneouskeypresses_func] << "private:\n"
    remapclass.code[:simultaneouskeypresses_variable].each do |variable|
      $outfile[:remapcode_simultaneouskeypresses_func] << "  KeyEventInputQueue::Remap #{variable[:name]};\n"
    end
    $outfile[:remapcode_simultaneouskeypresses_func] << "};\n"

    $outfile[:remapcode_simultaneouskeypresses_func] << "RemapClass_#{name} remapclass_#{name};\n"

    $outfile[:remapcode_simultaneouskeypresses_func] << "\n\n"
  end
end

$outfile[:remapcode_info] << "#ifndef REMAPCODE_INFO\n"
$outfile[:remapcode_info] << "#define REMAPCODE_INFO\n"
$outfile[:remapcode_info] << "enum {\n"
$outfile[:remapcode_info] << "  MAXNUM_REMAPFUNC_KEY = #{$func[:key].uniq.count},\n"
$outfile[:remapcode_info] << "  MAXNUM_REMAPFUNC_CONSUMER = #{$func[:consumer].uniq.count},\n"
$outfile[:remapcode_info] << "  MAXNUM_REMAPFUNC_POINTING = #{$func[:pointing].uniq.count},\n"
$outfile[:remapcode_info] << "};\n"
$outfile[:remapcode_info] << "#endif\n"

# set higher priority to notsave.* (ex. Vi Mode)
remapfunc_key_notsave = ''
remapfunc_key_other = ''
$func[:key].uniq.each do |name|
  text = ''
  if /^passthrough_/ =~ name then
    text += "if (config.#{name}) {\n"
  else
    text += "if (config.#{name} && ! config.notsave_passthrough) {\n"
  end
  text += "  listRemapFunc_key[listRemapFunc_key_size] = GeneratedCode::RemapClass_#{name}::remap_key;\n"
  text += "  ++listRemapFunc_key_size;\n"
  text += "}\n"
  if /^notsave_/ =~ name then
    remapfunc_key_notsave += text
  else
    remapfunc_key_other += text
  end
end
$outfile[:remapcode_refresh_remapfunc_key] << remapfunc_key_notsave
$outfile[:remapcode_refresh_remapfunc_key] << remapfunc_key_other

$func[:consumer].uniq.each do |name|
  if /^passthrough_/ =~ name then
    $outfile[:remapcode_refresh_remapfunc_consumer] << "if (config.#{name}) {\n"
  else
    $outfile[:remapcode_refresh_remapfunc_consumer] << "if (config.#{name} && ! config.notsave_passthrough) {\n"
  end
  $outfile[:remapcode_refresh_remapfunc_consumer] << "  listRemapFunc_consumer[listRemapFunc_consumer_size] = GeneratedCode::RemapClass_#{name}::remap_consumer;\n"
  $outfile[:remapcode_refresh_remapfunc_consumer] << "  ++listRemapFunc_consumer_size;\n"
  $outfile[:remapcode_refresh_remapfunc_consumer] << "}\n"
end
$func[:pointing].uniq.each do |name|
  if /^passthrough_/ =~ name then
    $outfile[:remapcode_refresh_remapfunc_pointing] << "if (config.#{name}) {\n"
  else
    $outfile[:remapcode_refresh_remapfunc_pointing] << "if (config.#{name} && ! config.notsave_passthrough) {\n"
  end
  $outfile[:remapcode_refresh_remapfunc_pointing] << "  listRemapFunc_pointing[listRemapFunc_pointing_size] = GeneratedCode::RemapClass_#{name}::remap_pointing;\n"
  $outfile[:remapcode_refresh_remapfunc_pointing] << "  ++listRemapFunc_pointing_size;\n"
  $outfile[:remapcode_refresh_remapfunc_pointing] << "}\n"
end

$outfile[:remapcode_simultaneouskeypresses_func] << "KeyEventInputQueue::RemapClass* listRemapClass_simultaneouskeypresses[] = {\n"
$func[:simultaneouskeypresses].uniq.each do |name|
  $outfile[:remapcode_simultaneouskeypresses_func] << "  &remapclass_#{name},\n"
end
$outfile[:remapcode_simultaneouskeypresses_func] << "  NULL,\n"
$outfile[:remapcode_simultaneouskeypresses_func] << "};\n"

$outfile.each do |name,file|
  file.close
end

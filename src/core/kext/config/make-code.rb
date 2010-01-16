#!/usr/bin/ruby
# -*- coding: undecided -*-

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
}

$func = {
  :key => [],
  :consumer => [],
  :pointing => [],
}

# ======================================================================
def getextrakey(key)
  case key
  when 'HOME'
    'CURSOR_LEFT'
  when 'END'
    'CURSOR_RIGHT'
  when 'PAGEUP'
    'CURSOR_UP'
  when 'PAGEDOWN'
    'CURSOR_DOWN'
  when 'FORWARD_DELETE'
    'DELETE'
  else
    nil
  end
end

def preprocess(listAutogen)
  modify = false

  list = []
  listAutogen.each do |autogen|
    if /VK_(COMMAND|CONTROL|SHIFT|OPTION)/ =~ autogen then
      key = $1
      list << autogen.gsub(/VK_#{key}/, "ModifierFlag::#{key}_L")
      list << autogen.gsub(/VK_#{key}/, "ModifierFlag::#{key}_R")
      modify = true
    elsif /VK_MOD_CCOS_L/ =~ autogen then
      list << autogen.gsub(/VK_MOD_CCOS_L/, "ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::OPTION_L | ModifierFlag::SHIFT_L")
      modify = true
    elsif /VK_MOD_CCS_L/ =~ autogen then
      list << autogen.gsub(/VK_MOD_CCS_L/, "ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L")
      modify = true
    elsif /FROMKEYCODE_(HOME|END|PAGEUP|PAGEDOWN|FORWARD_DELETE)\s*,\s*ModifierFlag::/ =~ autogen then
      key = $1
      extrakey = getextrakey(key)
      list << autogen.gsub(/FROMKEYCODE_#{key}\s*,/, "KeyCode::#{key},")
      list << autogen.gsub(/FROMKEYCODE_#{key}\s*,/, "KeyCode::#{extrakey}, ModifierFlag::FN |")
      modify = true
    elsif /FROMKEYCODE_(HOME|END|PAGEUP|PAGEDOWN|FORWARD_DELETE)/ =~ autogen then
      key = $1
      extrakey = getextrakey(key)
      list << autogen.gsub(/FROMKEYCODE_#{key}\s*,/, "KeyCode::#{key},")
      list << autogen.gsub(/FROMKEYCODE_#{key}\s*,/, "KeyCode::#{extrakey}, ModifierFlag::FN,")
      modify = true
    else
      list << autogen
    end
  end

  if modify then
    list = preprocess(list)
  end

  list
end

# ======================================================================
def parsefilter(block)
  filter = []

  if /<not>(.+?)<\/not>/m =~ block then
    $1.split(/,/).each do |f|
      filter << "if (remapParams.workspacedata.type == KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}) break;"
    end
  end
  if /<only>(.+?)<\/only>/m =~ block then
    tmp = []
    $1.split(/,/).each do |f|
      tmp << "(remapParams.workspacedata.type != KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip})"
    end
    filter << "if (#{tmp.join(' && ')}) break;"
  end
  if /<keyboardtype_not>(.+?)<\/keyboardtype_not>/m =~ block then
    $1.split(/,/).each do |f|
      filter << "if (remapParams.params.keyboardType == KeyboardType::#{f.strip}) break;"
    end
  end
  if /<keyboardtype_only>(.+?)<\/keyboardtype_only>/m =~ block then
    tmp = []
    $1.split(/,/).each do |f|
      tmp << "(remapParams.params.keyboardType != KeyboardType::#{f.strip})"
    end
    filter << "if (#{tmp.join(' && ')}) break;"
  end
  if /<(inputmode|inputmodedetail)_not>(.+?)<\/(inputmode|inputmodedetail)_not>/m =~ block then
    inputmodetype = $1
    $2.split(/,/).each do |f|
      filter << "if (remapParams.workspacedata.#{inputmodetype} == KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}) break;"
    end
  end
  if /<(inputmode|inputmodedetail)_only>(.+?)<\/(inputmode|inputmodedetail)_only>/m =~ block then
    inputmodetype = $1
    tmp = []
    $2.split(/,/).each do |f|
      tmp << "(remapParams.workspacedata.#{inputmodetype} != KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip})"
    end
    filter << "if (#{tmp.join(' && ')}) break;"
  end

  code = ''
  filter.each do |f|
    code += "    #{f}\n"
  end
  code
end

def parseautogen(name, block, out)
  listAutogen = block.scan(/<autogen>(.+?)<\/autogen>/m)
  return if listAutogen.empty?

  code_key = []
  code_consumer = []
  code_pointing = []
  code_variable = []

  filter = parsefilter(block)

  listAutogen = listAutogen.map{|autogen| autogen[0]}
  listAutogen = preprocess(listAutogen)
  listAutogen.each do |autogen|
    if /^--(.+?)-- (.+)$/ =~ autogen then
      type = $1
      params = $2
      out[:autogen_index] += 1

      case type
      when 'SetKeyboardType'
        $outfile[:remapcode_keyboardtype] << "if (config.#{name}) {\n"
        # XXX: indent me!
        $outfile[:remapcode_keyboardtype] << "keyboardType = #{params}.get();\n"
        $outfile[:remapcode_keyboardtype] << "}\n"

      when 'KeyToKey'
        code_variable << ['RemapUtil::KeyToKey', "keytokey#{out[:autogen_index]}_"]
        code_key << "if (keytokey#{out[:autogen_index]}_.remap(remapParams, #{params})) break;"
        $func[:key] << name

      when 'DoublePressModifier'
        code_variable << ['DoublePressModifier', "doublepressmodifier#{out[:autogen_index]}_"]
        code_key << "if (doublepressmodifier#{out[:autogen_index]}_.remap(remapParams, #{params})) break;"
        $func[:key] << name

      when 'IgnoreMultipleSameKeyPress'
        code_variable << ['IgnoreMultipleSameKeyPress', "ignoremultiplesamekeypress#{out[:autogen_index]}_"]
        code_key << "if (ignoremultiplesamekeypress#{out[:autogen_index]}_.remap(remapParams, #{params})) break;"
        $func[:key] << name

      when 'KeyToConsumer'
        code_variable << ['RemapUtil::KeyToConsumer', "keytoconsumer#{out[:autogen_index]}_"]
        code_key << "if (keytoconsumer#{out[:autogen_index]}_.remap(remapParams, #{params})) break;"
        $func[:key] << name

      when 'KeyToPointingButton'
        code_variable << ['RemapUtil::KeyToPointingButton', "keytopointing#{out[:autogen_index]}_"]
        code_key << "if (keytopointing#{out[:autogen_index]}_.remap(remapParams, #{params})) break;"
        $func[:key] << name

      when 'KeyOverlaidModifier'
        code_variable << ['KeyOverlaidModifier', "keyoverlaidmodifier#{out[:autogen_index]}_"]
        code_key << "if (keyoverlaidmodifier#{out[:autogen_index]}_.remap(remapParams, #{params})) break;"
        $func[:key] << name

      when 'KeyOverlaidModifierWithRepeat'
        code_variable << ['KeyOverlaidModifier', "keyoverlaidmodifier#{out[:autogen_index]}_"]
        code_key << "if (keyoverlaidmodifier#{out[:autogen_index]}_.remapWithRepeat(remapParams, #{params})) break;"
        $func[:key] << name

      when 'ModifierHoldingKeyToKey'
        code_variable << ['ModifierHoldingKeyToKey', "modifierholdingkeytokey#{out[:autogen_index]}_"]
        code_key << "if (modifierholdingkeytokey#{out[:autogen_index]}_.remap(remapParams, #{params})) break;"
        $func[:key] << name

      when 'ConsumerToKey'
        code_variable << ['RemapUtil::ConsumerToKey', "consumertokey#{out[:autogen_index]}_"]
        code_consumer << "if (consumertokey#{out[:autogen_index]}_.remap(remapParams, #{params})) break;"
        $func[:consumer] << name

      when 'ConsumerToConsumer'
        code_variable << ['RemapUtil::ConsumerToConsumer', "consumertoconsumer#{out[:autogen_index]}_"]
        code_consumer << "if (consumertoconsumer#{out[:autogen_index]}_.remap(remapParams, #{params})) break;"
        $func[:consumer] << name

      when 'PointingRelativeToScroll'
        code_variable << ['RemapUtil::PointingRelativeToScroll', "pointingrelativetoscroll#{out[:autogen_index]}_"]
        code_pointing << "if (pointingrelativetoscroll#{out[:autogen_index]}_.remap(remapParams, #{params})) break;"
        $func[:pointing] << name

      when 'PointingButtonToPointingButton'
        code_variable << ['RemapUtil::PointingButtonToPointingButton', "pointingbuttontopointingbutton#{out[:autogen_index]}_"]
        code_pointing << "if (pointingbuttontopointingbutton#{out[:autogen_index]}_.remap(remapParams, #{params})) break;"
        $func[:pointing] << name

      else
        print "%%% ERROR #{type} %%%\n#{item}\n"
        exit 1

      end
    end
  end

  unless code_key.empty? then
    out[:code_key] += "  do {\n"
    unless filter.empty? then
      out[:code_key] += "#{filter}\n"
    end
    code_key.each do |line|
      out[:code_key] += "    #{line}\n"
    end
    out[:code_key] += "  } while (false);\n"
  end
  unless code_consumer.empty? then
    out[:code_consumer] += "  do {\n"
    unless filter.empty? then
      out[:code_consumer] += "#{filter}\n"
    end
    code_consumer.each do |line|
      out[:code_consumer] += "    #{line}\n"
    end
    out[:code_consumer] += "  } while (false);\n"
  end
  unless code_pointing.empty? then
    out[:code_pointing] += "  do {\n"
    code_pointing.each do |line|
      out[:code_pointing] += "    #{line}\n"
    end
    out[:code_pointing] += "  } while (false);\n"
  end
  out[:code_variable] += code_variable
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

  # ======================================================================
  listBlock = item.scan(/<block>(.+?)<\/block>/m)
  listBlock = listBlock.map{|block| block[0]}
  item.gsub!(/<block>(.+?)<\/block>/m, '')
  listBlock << item

  out = {
    :code_key => '',
    :code_consumer => '',
    :code_pointing => '',
    :code_variable => [],
    :autogen_index => 0
  }

  listBlock.each do |block|
    parseautogen(name, block, out)
  end

  next if out[:code_key].empty? &&
    out[:code_consumer].empty? &&
    out[:code_pointing].empty? &&
    out[:code_variable].empty?

  $outfile[:remapcode_func] << "class RemapClass_#{name} {\n"
  $outfile[:remapcode_func] << "public:\n"
  unless out[:code_key].empty? then
    $outfile[:remapcode_func] << "static void remap_key(RemapParams &remapParams) {\n"
    $outfile[:remapcode_func] << out[:code_key]
    $outfile[:remapcode_func] << "}\n"
  end
  unless out[:code_consumer].empty? then
    $outfile[:remapcode_func] << "static void remap_consumer(RemapConsumerParams &remapParams) {\n"
    $outfile[:remapcode_func] << out[:code_consumer]
    $outfile[:remapcode_func] << "}\n"
  end
  unless out[:code_pointing].empty? then
    $outfile[:remapcode_func] << "static void remap_pointing(RemapPointingParams_relative &remapParams) {\n"
    $outfile[:remapcode_func] << out[:code_pointing]
    $outfile[:remapcode_func] << "}\n"
  end
  $outfile[:remapcode_func] << "\n"
  $outfile[:remapcode_func] << "private:\n"
  out[:code_variable].each do |variable|
    $outfile[:remapcode_func] << "  static #{variable[0]} #{variable[1]};\n"
  end
  $outfile[:remapcode_func] << "};\n"

  out[:code_variable].each do |variable|
    $outfile[:remapcode_func] << "#{variable[0]} RemapClass_#{name}::#{variable[1]};\n"
  end
  $outfile[:remapcode_func] << "\n\n"
end

$outfile[:remapcode_info] << "enum {\n"
$outfile[:remapcode_info] << "  MAXNUM_REMAPFUNC_KEY = #{$func[:key].uniq.count},\n"
$outfile[:remapcode_info] << "  MAXNUM_REMAPFUNC_CONSUMER = #{$func[:consumer].uniq.count},\n"
$outfile[:remapcode_info] << "  MAXNUM_REMAPFUNC_POINTING = #{$func[:pointing].uniq.count},\n"
$outfile[:remapcode_info] << "};\n"

$func[:key].uniq.each do |name|
  $outfile[:remapcode_refresh_remapfunc_key] << "if (config.#{name}) {\n"
  $outfile[:remapcode_refresh_remapfunc_key] << "  listRemapFunc_key[listRemapFunc_key_size] = GeneratedCode::RemapClass_#{name}::remap_key;\n"
  $outfile[:remapcode_refresh_remapfunc_key] << "  ++listRemapFunc_key_size;\n"
  $outfile[:remapcode_refresh_remapfunc_key] << "}\n"
end
$func[:consumer].uniq.each do |name|
  $outfile[:remapcode_refresh_remapfunc_consumer] << "if (config.#{name}) {\n"
  $outfile[:remapcode_refresh_remapfunc_consumer] << "  listRemapFunc_consumer[listRemapFunc_consumer_size] = GeneratedCode::RemapClass_#{name}::remap_consumer;\n"
  $outfile[:remapcode_refresh_remapfunc_consumer] << "  ++listRemapFunc_consumer_size;\n"
  $outfile[:remapcode_refresh_remapfunc_consumer] << "}\n"
end
$func[:pointing].uniq.each do |name|
  $outfile[:remapcode_refresh_remapfunc_pointing] << "if (config.#{name}) {\n"
  $outfile[:remapcode_refresh_remapfunc_pointing] << "  listRemapFunc_pointing[listRemapFunc_pointing_size] = GeneratedCode::RemapClass_#{name}::remap_pointing;\n"
  $outfile[:remapcode_refresh_remapfunc_pointing] << "  ++listRemapFunc_pointing_size;\n"
  $outfile[:remapcode_refresh_remapfunc_pointing] << "}\n"
end

$outfile.each do |name,file|
  file.close
end

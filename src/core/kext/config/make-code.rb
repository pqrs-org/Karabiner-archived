#!/usr/bin/ruby
# -*- coding: undecided -*-

require 'rexml/document'

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

# ----------------------------------------------------------------------
vars = ''
sysctl = ''
register = ''
unregister = ''
default = ''
func = {
  'key' => [],
  'consumer' => [],
  'pointing' => [],
}
code_keyboardtype = ''
code = ''

$stdin.read.scan(/<item>.+?<\/item>/m).each do |item|
  if /.*(<item>.+?<\/item>)/m =~ item then
    item = $1
  end

  name = nil

  if /<sysctl>([^\.]+?)\.(.+?)<\/sysctl>/m =~ item then
    name = "#{$1}_#{$2}"
    sysctl += "SYSCTL_INT(_keyremap4macbook_#{$1}, OID_AUTO, #{$2}, CTLTYPE_INT|CTLFLAG_RW, &(config.#{name}), 0, \"\");\n"
    register += "sysctl_register_oid(&sysctl__keyremap4macbook_#{name});\n"
    unregister += "sysctl_unregister_oid(&sysctl__keyremap4macbook_#{name});\n"
  end

  next if name.nil?

  vars += "int #{name};\n"

  if /<default>(.+?)<\/default>/m =~ item then
    default += "#{name} = #{$1};\n"
  end

  # ======================================================================
  # filter

  filter = ''

  if /<not>(.+?)<\/not>/m =~ item then
    $1.split(/,/).each do |f|
      filter += "  if (remapParams.workspacedata.type == KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}) return;\n"
    end
  end
  if /<only>(.+?)<\/only>/m =~ item then
    tmp = []
    $1.split(/,/).each do |f|
      tmp << "(remapParams.workspacedata.type != KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip})"
    end
    filter += "  if (#{tmp.join(' && ')}) return;\n"
  end
  if /<keyboardtype_only>(.+?)<\/keyboardtype_only>/m =~ item then
    tmp = []
    $1.split(/,/).each do |f|
      tmp << "(remapParams.params.keyboardType != KeyboardType::#{f.strip})"
    end
    filter += "  if (#{tmp.join(' && ')}) return;\n"
  end
  if /<(inputmode|inputmodedetail)_not>(.+?)<\/(inputmode|inputmodedetail)_not>/m =~ item then
    inputmodetype = $1
    $2.split(/,/).each do |f|
      filter += "  if (remapParams.workspacedata.#{inputmodetype} == KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}) return;\n"
    end
  end
  if /<(inputmode|inputmodedetail)_only>(.+?)<\/(inputmode|inputmodedetail)_only>/m =~ item then
    inputmodetype = $1
    tmp = []
    $2.split(/,/).each do |f|
      tmp << "(remapParams.workspacedata.#{inputmodetype} != KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip})"
    end
    filter += "  if (#{tmp.join(' && ')}) return;\n"
  end

  # ======================================================================
  # autogen

  listAutogen = item.scan(/<autogen>(.+?)<\/autogen>/m)
  next if listAutogen.empty?

  autogen_index = 0
  code_key = []
  code_consumer = []
  code_pointing = []
  code_variable = []

  listAutogen = listAutogen.map{|autogen| autogen[0]}
  listAutogen = preprocess(listAutogen)
  listAutogen.each do |autogen|
    if /^--(.+?)-- (.+)$/ =~ autogen then
      type = $1
      params = $2
      autogen_index += 1

      case type
      when 'SetKeyboardType'
        code_keyboardtype += "if (config.#{name}) {\n"
        code_keyboardtype += "keyboardType = #{params}.get();\n"
        code_keyboardtype += "}\n"

      when 'KeyToKey'
        code_variable << ['RemapUtil::KeyToKey', "keytokey#{autogen_index}_"]
        code_key << "if (keytokey#{autogen_index}_.remap(remapParams, #{params})) return;"
        func['key'] << name

      when 'DoublePressModifier'
        code_variable << ['DoublePressModifier', "doublepressmodifier#{autogen_index}_"]
        code_key << "if (doublepressmodifier#{autogen_index}_.remap(remapParams, #{params})) return;"
        func['key'] << name

      when 'IgnoreMultipleSameKeyPress'
        code_variable << ['IgnoreMultipleSameKeyPress', "ignoremultiplesamekeypress#{autogen_index}_"]
        code_key << "if (ignoremultiplesamekeypress#{autogen_index}_.remap(remapParams, #{params})) return;"
        func['key'] << name

      when 'KeyToComsumer'
        code_variable << ['RemapUtil::KeyToConsumer', "keytoconsumer#{autogen_index}_"]
        code_key << "if (keytoconsumer#{autogen_index}_.remap(remapParams, #{params})) return;"
        func['key'] << name

      when 'KeyToPointingButton'
        code_variable << ['RemapUtil::KeyToPointingButton', "keytopointing#{autogen_index}_"]
        code_key << "if (keytopointing#{autogen_index}_.remap(remapParams, #{params})) return;"
        func['key'] << name

      when 'KeyOverlaidModifier'
        code_variable << ['KeyOverlaidModifier', "keyoverlaidmodifier#{autogen_index}_"]
        code_key << "if (keyoverlaidmodifier#{autogen_index}_.remap(remapParams, #{params})) return;"
        func['key'] << name

      when 'KeyOverlaidModifierWithRepeat'
        code_variable << ['KeyOverlaidModifier', "keyoverlaidmodifier#{autogen_index}_"]
        code_key << "if (keyoverlaidmodifier#{autogen_index}_.remapWithRepeat(remapParams, #{params})) return;"
        func['key'] << name

      when 'ModifierHoldingKeyToKey'
        code_variable << ['ModifierHoldingKeyToKey', "modifierholdingkeytokey#{autogen_index}_"]
        code_key << "if (modifierholdingkeytokey#{autogen_index}_.remap(remapParams, #{params})) return;"
        func['key'] << name

      when 'ConsumerToKey'
        code_variable << ['RemapUtil::ConsumerToKey', "consumertokey#{autogen_index}_"]
        code_consumer << "if (consumertokey#{autogen_index}_.remap(remapParams, #{params})) return;"
        func['consumer'] << name

      when 'ConsumerToConsumer'
        code_variable << ['RemapUtil::ConsumerToConsumer', "consumertoconsumer#{autogen_index}_"]
        code_consumer << "if (consumertoconsumer#{autogen_index}_.remap(remapParams, #{params})) return;"
        func['consumer'] << name

      when 'PointingRelativeToScroll'
        code_variable << ['RemapUtil::PointingRelativeToScroll', "pointingrelativetoscroll#{autogen_index}_"]
        code_pointing << "if (pointingrelativetoscroll#{autogen_index}_.remap(remapParams, #{params})) return;"
        func['pointing'] << name

      when 'PointingButtonToPointingButton'
        code_variable << ['RemapUtil::PointingButtonToPointingButton', "pointingbuttontopointingbutton#{autogen_index}_"]
        code_pointing << "if (pointingbuttontopointingbutton#{autogen_index}_.remap(remapParams, #{params})) return;"
        func['pointing'] << name

      else
        print "%%% ERROR #{type} %%%\n#{item}\n"
        exit 1

      end
    end
  end

  check = "  if (! config.#{name}) return;\n"

  code += "class RemapClass_#{name} {\n"
  code += "public:\n"
  unless code_key.empty? then
    code += "static void remap_key(RemapParams &remapParams) {\n#{check}#{filter}\n"
    code_key.each do |line|
      code += "  #{line}\n"
    end
    code += "}\n"
  end
  unless code_consumer.empty? then
    code += "static void remap_consumer(RemapConsumerParams &remapParams) {\n#{check}#{filter}\n"
    code_consumer.each do |line|
      code += "  #{line}\n"
    end
    code += "}\n"
  end
  unless code_pointing.empty? then
    code += "static void remap_pointing(RemapPointingParams_relative &remapParams) {\n#{check}\n"
    code_pointing.each do |line|
      code += "  #{line}\n"
    end
    code += "}\n"
  end
  code += "\n"
  code += "private:\n"
  code_variable.each do |variable|
    code += "  static #{variable[0]} #{variable[1]};\n"
  end
  code += "};\n"

  code_variable.each do |variable|
    code += "#{variable[0]} RemapClass_#{name}::#{variable[1]};\n"
  end
  code += "\n\n"
end

open('output/include.config.hpp', 'w') do |f|
  f.puts vars
end
open('output/include.config_SYSCTL.cpp', 'w') do |f|
  f.puts sysctl
end
open('output/include.config_register.cpp', 'w') do |f|
  f.puts register
end
open('output/include.config_unregister.cpp', 'w') do |f|
  f.puts unregister
end
open('output/include.config.default.hpp', 'w') do |f|
  f.puts default
end
open('output/include.remapcode_keyboardtype.hpp', 'w') do |f|
  f.puts code_keyboardtype
end
open('output/include.remapcode_func.cpp', 'w') do |f|
  f.puts code
end
open('output/include.remapcode_call.cpp', 'w') do |f|
  func['key'].uniq.each do |call|
    f.puts "GeneratedCode::RemapClass_#{call}::remap_key(remapParams);\n"
  end
end
open('output/include.remapcode_call_consumer.cpp', 'w') do |f|
  func['consumer'].uniq.each do |call|
    f.puts "GeneratedCode::RemapClass_#{call}::remap_consumer(remapParams);\n"
  end
end
open('output/include.remapcode_call_pointing_relative.cpp', 'w') do |f|
  func['pointing'].uniq.each do |call|
    f.puts "GeneratedCode::RemapClass_#{call}::remap_pointing(remapParams);\n"
  end
end

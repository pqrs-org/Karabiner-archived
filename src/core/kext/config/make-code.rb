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
code = ''

$stdin.read.scan(/<item>.+?<\/item>/m).each do |item|
  if /.*(<item>.+?<\/item>)/m =~ item then
    item = $1
  end

  name = nil
  filter = ''
  code_key = ''
  code_consumer = ''
  code_pointing = ''

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

  if /<not>(.+?)<\/not>/m =~ item then
    $1.split(/,/).each do |f|
      filter += "if (remapParams.workspacedata.type == KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}) return;\n"
    end
  end
  if /<only>(.+?)<\/only>/m =~ item then
    tmp = []
    $1.split(/,/).each do |f|
      tmp << "(remapParams.workspacedata.type != KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip})"
    end
    filter += "if (#{tmp.join(' && ')}) return;\n"
  end
  if /<keyboardtype_only>(.+?)<\/keyboardtype_only>/m =~ item then
    tmp = []
    $1.split(/,/).each do |f|
      tmp << "(remapParams.params.keyboardType != KeyboardType::#{f.strip})"
    end
    filter += "if (#{tmp.join(' && ')}) return;\n"
  end
  if /<(inputmode|inputmodedetail)_not>(.+?)<\/(inputmode|inputmodedetail)_not>/m =~ item then
    inputmodetype = $1
    $2.split(/,/).each do |f|
      filter += "if (remapParams.workspacedata.#{inputmodetype} == KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}) return;\n"
    end
  end
  if /<(inputmode|inputmodedetail)_only>(.+?)<\/(inputmode|inputmodedetail)_only>/m =~ item then
    inputmodetype = $1
    tmp = []
    $2.split(/,/).each do |f|
      tmp << "(remapParams.workspacedata.#{inputmodetype} != KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip})"
    end
    filter += "if (#{tmp.join(' && ')}) return;\n"
  end

  listAutogen = item.scan(/<autogen>(.+?)<\/autogen>/m)
  unless listAutogen.empty? then
    listAutogen = listAutogen.map{|autogen| autogen[0]}
    listAutogen = preprocess(listAutogen)
    listAutogen.each do |autogen|
      if /^--(.+?)-- (.+)$/ =~ autogen then
        type = $1
        params = $2

        case type
        when 'KeyToKey'
          code_key += "{\n"
          code_key += "static RemapUtil::KeyToKey keytokey;\n"
          code_key += "if (keytokey.remap(remapParams, #{params})) return;\n"
          code_key += "}\n"
          func['key'] << name

        when 'DoublePressModifier'
          code_key += "{\n"
          code_key += "static DoublePressModifier dpm;\n"
          code_key += "if (dpm.remap(remapParams, #{params})) return;\n"
          code_key += "}\n"
          func['key'] << name

        when 'IgnoreMultipleSameKeyPress'
          code_key += "{\n"
          code_key += "static IgnoreMultipleSameKeyPress imsp;\n"
          code_key += "if (imsp.remap(remapParams, #{params})) return;\n"
          code_key += "}\n"
          func['key'] << name

        when 'KeyToComsumer'
          code_key += "{\n"
          code_key += "static RemapUtil::KeyToConsumer keytoconsumer;\n"
          code_key += "if (keytoconsumer.remap(remapParams, #{params})) return;\n"
          code_key += "}\n"
          func['key'] << name

        when 'KeyToPointingButton'
          code_key += "{\n"
          code_key += "static RemapUtil::KeyToPointingButton keytopointing;\n"
          code_key += "if (keytopointing.remap(remapParams, #{params})) return;\n"
          code_key += "}\n"
          func['key'] << name

        when 'KeyOverlaidModifier'
          code_key += "{\n"
          code_key += "static KeyOverlaidModifier kom;\n"
          code_key += "if (kom.remap(remapParams, #{params})) return;\n"
          code_key += "}\n"
          func['key'] << name

        when 'KeyOverlaidModifierWithRepeat'
          code_key += "{\n"
          code_key += "static KeyOverlaidModifier kom;\n"
          code_key += "if (kom.remapWithRepeat(remapParams, #{params})) return;\n"
          code_key += "}\n"
          func['key'] << name

        when 'ModifierHoldingKeyToKey'
          code_key += "{\n"
          code_key += "static ModifierHoldingKeyToKey mhkk;\n"
          code_key += "if (mhkk.remap(remapParams, #{params})) return;\n"
          code_key += "}\n"
          func['key'] << name

        when 'ConsumerToKey'
          code_consumer += "{\n"
          code_consumer += "static RemapUtil::ConsumerToKey consumertokey;\n"
          code_consumer += "if (consumertokey.remap(remapParams, #{params})) return;\n"
          code_consumer += "}\n"
          func['consumer'] << name

        when 'ConsumerToConsumer'
          code_consumer += "{\n"
          code_consumer += "static RemapUtil::ConsumerToConsumer consumertoconsumer;\n"
          code_consumer += "if (consumertoconsumer.remap(remapParams, #{params})) return;\n"
          code_consumer += "}\n"
          func['consumer'] << name

        when 'PointingRelativeToScroll'
          code_pointing += "{\n"
          code_pointing += "static RemapUtil::PointingRelativeToScroll brts;\n"
          code_pointing += "if (brts.remap(remapParams, #{params})) return;\n"
          code_pointing += "}\n"
          func['pointing'] << name

        when 'PointingButtonToPointingButton'
          code_pointing += "{\n"
          code_pointing += "static RemapUtil::PointingButtonToPointingButton pbtopb;\n"
          code_pointing += "if (pbtopb.remap(remapParams, #{params})) return;\n"
          code_pointing += "}\n"
          func['pointing'] << name

        else
          print "%%% ERROR #{type} %%%\n#{item}\n"
          exit 1

        end
      end
    end
  end

  check = "if (! config.#{name}) return;"
  unless code_key.empty? then
    code += "static void #{name}(RemapParams &remapParams) {\n#{check}\n#{filter}\n#{code_key}\n}\n\n\n"
  end
  unless code_consumer.empty? then
    code += "static void #{name}(RemapConsumerParams &remapParams) {\n#{check}\n#{filter}\n#{code_consumer}\n}\n\n\n"
  end
  unless code_pointing.empty? then
    code += "static void #{name}(RemapPointingParams_relative &remapParams) {\n#{check}\n#{code_pointing}\n}\n\n\n"
  end
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
open('output/include.remapcode_func.cpp', 'w') do |f|
  f.puts code
end
open('output/include.remapcode_call.cpp', 'w') do |f|
  func['key'].uniq.each do |call|
    f.puts "GeneratedCode::#{call}(remapParams);\n"
  end
end
open('output/include.remapcode_call_consumer.cpp', 'w') do |f|
  func['consumer'].uniq.each do |call|
    f.puts "GeneratedCode::#{call}(remapParams);\n"
  end
end
open('output/include.remapcode_call_pointing_relative.cpp', 'w') do |f|
  func['pointing'].uniq.each do |call|
    f.puts "GeneratedCode::#{call}(remapParams);\n"
  end
end

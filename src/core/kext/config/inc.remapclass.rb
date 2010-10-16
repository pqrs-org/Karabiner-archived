#!/usr/bin/ruby
# -*- coding: utf-8 -*-

require 'inc.filter.rb'
require 'inc.keycode.rb'

class RemapClass
  @@entries = []
  @@simultaneous_keycode_index = 0
  @@variable_index = 0

  def RemapClass.get_entries
    return @@entries
  end

  def RemapClass.reset_variable_index
    @@variable_index = 0
  end

  def initialize(name)
    @name = name

    @code = {
      # functions
      :initialize                   => '',
      :remap_setkeyboardtype        => '',
      :get_statusmessage            => '',
    }

    @@entries << {
      :name => @name,
      :initialize => [],
      :terminate => [],
      :remap_setkeyboardtype        => [],
      :remap_key                    => [],
      :remap_consumer               => [],
      :remap_pointing               => [],
      :remap_simultaneouskeypresses => [],
      :remap_dropkeyafterremap      => [],
      :get_statusmessage            => [],
      :enabled                      => [],
    }
  end
  attr_accessor :name, :filter, :code

  def append_to_code_initialize(params, operation)
    # We split the initialize function per value.
    # If a large number of values exist, the kernel stack is wasted when the monolithic initialize function is called.
    # So, we split it.

    args = []
    args << "static_cast<unsigned int>(BRIDGE_REMAPTYPE_#{operation.upcase})"

    params.split(/,/).each do |p|
      datatype = nil
      newval = []
      p.split(/\|/).each do |value|
        value.strip!
        next if value.empty?
        newdatatype = nil
        if /^KeyCode::(.+)$/ =~ value then
          newdatatype = 'static_cast<unsigned int>(BRIDGE_DATATYPE_KEYCODE)'
        elsif /^ModifierFlag::(.+)$/ =~ value then
          newdatatype = 'static_cast<unsigned int>(BRIDGE_DATATYPE_FLAGS)'
        elsif /^ConsumerKeyCode::(.+)$/ =~ value then
          newdatatype = 'static_cast<unsigned int>(BRIDGE_DATATYPE_CONSUMERKEYCODE)'
        elsif /^PointingButton::(.+)$/ =~ value then
          newdatatype = 'static_cast<unsigned int>(BRIDGE_DATATYPE_POINTINGBUTTON)'
        elsif /^Option::(.+)$/ =~ value then
          newdatatype = 'static_cast<unsigned int>(BRIDGE_DATATYPE_OPTION)'
        else
          print "[ERROR] unknown datatype #{value}\n"
          throw :exit
        end

        if (not datatype.nil?) and (datatype != newdatatype) then
          throw :exit
        end

        datatype = newdatatype
        if KeyCode[value].nil? then
          print "[ERROR] unknown keycode #{value}\n"
          throw :exit
        end
        newval << KeyCode[value]
      end
      args << datatype
      args << newval.join('|')
    end

    @code[:initialize] += "{\n"
    @code[:initialize] += "  const unsigned int vec[] = { #{args.join(',')} };\n"
    @code[:initialize] += "  value_[#{@@variable_index}].initialize_remap(vec, sizeof(vec) / sizeof(vec[0]));\n"
    @code[:initialize] += "}\n"
  end
  protected :append_to_code_initialize

  def handle_autogen(autogen_node)
    if /^--(.+?)-- (.+)/ =~ autogen_node.inner_xml.strip then
      operation = $1
      params = $2

      case operation
      when 'SetKeyboardType'
        @code[:remap_setkeyboardtype] += "keyboardType = #{params}.get(); return;\n"

      when 'DropKeyAfterRemap'
        append_to_code_initialize(params, operation)

      when 'ShowStatusMessage'
        @code[:get_statusmessage] += "return #{params};\n"

      when 'SimultaneousKeyPresses'
        params = "KeyCode::VK_SIMULTANEOUSKEYPRESSES_#{@@simultaneous_keycode_index}, " + params
        @@simultaneous_keycode_index += 1
        append_to_code_initialize(params, operation)

      when 'KeyToKey', 'KeyToConsumer', 'KeyToPointingButton', 'DoublePressModifier', 'HoldingKeyToKey', 'IgnoreMultipleSameKeyPress', 'KeyOverlaidModifier'
        append_to_code_initialize(params, operation)

      when 'ConsumerToConsumer', 'ConsumerToKey'
        append_to_code_initialize(params, operation)

      when 'PointingButtonToPointingButton', 'PointingButtonToKey', 'PointingRelativeToScroll'
        append_to_code_initialize(params, operation)

      else
        print "%%% ERROR #{type} %%%\n#{l}\n"
        exit 1
      end

      @@variable_index += 1
    end
  end

  def empty?
    @code.each do |k, v|
      return false unless v.empty?
    end
    return true
  end

  def to_code(item_node)
    item_node.find('.//autogen').each do |autogen_node|
      filter = Filter.new

      @code[:initialize] += "{\n"
      @code[:initialize] += "  const unsigned int vec[] = { #{filter.to_code(item_node, autogen_node)} };\n"
      @code[:initialize] += "  value_[#{@@variable_index}].initialize_filter(vec, sizeof(vec) / sizeof(vec[0]));\n"
      @code[:initialize] += "}\n"

      handle_autogen(autogen_node)
    end

    # ----------------------------------------
    return '' if empty?

    classname = "RemapClass_#{@name}"

    code  = "class #{classname} {\n"
    code += "public:\n"

    # ----------------------------------------------------------------------
    code += "static void initialize(void) {\n"
    code += @code[:initialize]
    code += "}\n"
    @@entries[-1][:initialize] << "RemapClass_#{@name}::initialize"

    code += "static void terminate(void) {\n"
    code += "  for (size_t i = 0; i < sizeof(value_) / sizeof(value_[0]); ++i) {\n"
    code += "    value_[i].terminate();\n"
    code += "  }\n"
    code += "}\n"
    @@entries[-1][:terminate] << "RemapClass_#{@name}::terminate"

    # ----------------------------------------------------------------------
    unless @code[:remap_setkeyboardtype].empty? then
      code += "static void remap_setkeyboardtype(KeyboardType& keyboardType) {\n"
      code += @code[:remap_setkeyboardtype]
      code += "}\n"

      @@entries[-1][:remap_setkeyboardtype] << "#{classname}::remap_setkeyboardtype"
    else
      @@entries[-1][:remap_setkeyboardtype] << "NULL"
    end

    # ----------------------------------------------------------------------
    if @@variable_index > 0 then
      code += "static void remap_key(RemapParams& remapParams) {\n"
      code += "  for (int i = 0; i < #{@@variable_index}; ++i) {\n"
      code += "    if (value_[i].remap(remapParams)) return;\n"
      code += "  }\n"
      code += "}\n"

      @@entries[-1][:remap_key] << "#{classname}::remap_key"
    else
      @@entries[-1][:remap_key] << "NULL"
    end

    # ----------------------------------------------------------------------
    if @@variable_index > 0 then
      code += "static void remap_consumer(RemapConsumerParams& remapParams) {\n"
      code += "  for (int i = 0; i < #{@@variable_index}; ++i) {\n"
      code += "    if (value_[i].remap(remapParams)) return;\n"
      code += "  }\n"
      code += "}\n"

      @@entries[-1][:remap_consumer] << "#{classname}::remap_consumer"
    else
      @@entries[-1][:remap_consumer] << "NULL"
    end

    # ----------------------------------------------------------------------
    if @@variable_index > 0 then
      code += "static void remap_pointing(RemapPointingParams_relative& remapParams) {\n"
      code += "  for (int i = 0; i < #{@@variable_index}; ++i) {\n"
      code += "    if (value_[i].remap(remapParams)) return;\n"
      code += "  }\n"
      code += "}\n"

      @@entries[-1][:remap_pointing] << "#{classname}::remap_pointing"
    else
      @@entries[-1][:remap_pointing] << "NULL"
    end

    # ----------------------------------------------------------------------
    if @@variable_index > 0 then
      code += "static void remap_simultaneouskeypresses(void) {\n"
      code += "  for (int i = 0; i < #{@@variable_index}; ++i) {\n"
      code += "    value_[i].remap_SimultaneousKeyPresses();\n"
      code += "  }\n"
      code += "}\n"

      @@entries[-1][:remap_simultaneouskeypresses] << "#{classname}::remap_simultaneouskeypresses"
    else
      @@entries[-1][:remap_simultaneouskeypresses] << "NULL"
    end

    # ----------------------------------------------------------------------
    unless @code[:get_statusmessage].empty? then
      code += "static const char* get_statusmessage(void) {\n"
      code += @code[:get_statusmessage]
      code += "}\n"

      @@entries[-1][:get_statusmessage] << "#{classname}::get_statusmessage"
    else
      @@entries[-1][:get_statusmessage] << "NULL"
    end

    # ----------------------------------------------------------------------
    if @@variable_index > 0 then
      code += "static bool remap_dropkeyafterremap(const Params_KeyboardEventCallBack& params) {\n"
      code += "  if (! enabled()) return false;\n"
      code += "  for (int i = 0; i < #{@@variable_index}; ++i) {\n"
      code += "    if (value_[i].drop(params)) return true;\n"
      code += "  }\n"
      code += "  return false;\n"
      code += "}\n"

      @@entries[-1][:remap_dropkeyafterremap] << "#{classname}::remap_dropkeyafterremap"
    end

    # ----------------------------------------------------------------------
    code   += "static bool enabled(void) {\n"
    if @name == 'notsave_passthrough' then
      code += "return config.notsave_passthrough;\n"
    elsif /^passthrough_/ =~ @name then
      code += "return config.enabled_flags[#{KeyCode.ConfigIndex(@name)}];\n"
    else
      code += "return config.enabled_flags[#{KeyCode.ConfigIndex(@name)}] && ! config.notsave_passthrough;\n"
    end
    code   += "}\n"

    @@entries[-1][:enabled] << "#{classname}::enabled"

    # ----------------------------------------
    code += "\n"
    code += "private:\n"
    code += "static RemapClass::Item value_[#{@@variable_index}];\n"
    code += "};\n"

    code += "RemapClass::Item #{classname}::value_[#{@@variable_index}];\n"
    code += "\n\n"

    code
  end
end

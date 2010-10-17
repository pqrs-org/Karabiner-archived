#!/usr/bin/ruby
# -*- coding: utf-8 -*-

require 'inc.filter.rb'
require 'inc.keycode.rb'

class RemapClass
  @@simultaneous_keycode_index = 0
  @@variable_index = 0

  def RemapClass.reset_variable_index
    @@variable_index = 0
  end

  def initialize(name)
    @name = name

    @code = {
      # functions
      :initialize_vector => [],
      :setkeyboardtype   => nil,
      :statusmessage     => 'NULL',
    }
  end
  attr_accessor :name, :filter, :code

  def append_to_code_initialize_vector(params, operation, filtervec)
    args = []
    args << "BRIDGE_REMAPTYPE_#{operation.upcase}"

    params.split(/,/).each do |p|
      datatype = nil
      newval = []
      p.split(/\|/).each do |value|
        value.strip!
        next if value.empty?
        newdatatype = nil
        if /^KeyCode::(.+)$/ =~ value then
          newdatatype = 'BRIDGE_DATATYPE_KEYCODE'
        elsif /^ModifierFlag::(.+)$/ =~ value then
          newdatatype = 'BRIDGE_DATATYPE_FLAGS'
        elsif /^ConsumerKeyCode::(.+)$/ =~ value then
          newdatatype = 'BRIDGE_DATATYPE_CONSUMERKEYCODE'
        elsif /^PointingButton::(.+)$/ =~ value then
          newdatatype = 'BRIDGE_DATATYPE_POINTINGBUTTON'
        elsif /^Option::(.+)$/ =~ value then
          newdatatype = 'BRIDGE_DATATYPE_OPTION'
        else
          print "[ERROR] unknown datatype #{value}\n"
          throw :exit
        end

        if (not datatype.nil?) and (datatype != newdatatype) then
          throw :exit
        end

        datatype = newdatatype
        newval << KeyCode[value]
      end
      args << datatype
      args << newval.join('|')
    end

    @code[:initialize_vector] << args.count
    @code[:initialize_vector] += args

    unless filtervec.empty? then
      @code[:initialize_vector] += filtervec
    end
  end
  protected :append_to_code_initialize_vector

  def handle_autogen(autogen_node, filtervec)
    if /^--(.+?)-- (.+)/ =~ autogen_node.inner_xml.strip then
      operation = $1
      params = $2

      case operation
      when 'ShowStatusMessage'
        @code[:statusmessage] = params

      when 'SetKeyboardType'
        @code[:setkeyboardtype] = KeyCode[params]

      when 'SimultaneousKeyPresses'
        params = "KeyCode::VK_SIMULTANEOUSKEYPRESSES_#{@@simultaneous_keycode_index}, " + params
        @@simultaneous_keycode_index += 1
        append_to_code_initialize_vector(params, operation, filtervec)
        @@variable_index += 1

      when 'KeyToKey', 'KeyToConsumer', 'KeyToPointingButton', 'DoublePressModifier', 'HoldingKeyToKey', 'IgnoreMultipleSameKeyPress', 'KeyOverlaidModifier',
        'ConsumerToConsumer', 'ConsumerToKey', 'PointingButtonToPointingButton', 'PointingButtonToKey', 'PointingRelativeToScroll', 'DropKeyAfterRemap'
        append_to_code_initialize_vector(params, operation, filtervec)
        @@variable_index += 1

      else
        print "%%% ERROR #{type} %%%\n#{l}\n"
        exit 1
      end
    end
  end

  def to_code(item_node, outfile)
    item_node.find('.//autogen').each do |autogen_node|
      filter = Filter.new
      filtervec = filter.to_vector(item_node, autogen_node)
      handle_autogen(autogen_node, filtervec)
    end

    # ----------------------------------------
    outfile << "static const unsigned int remapclass_#{@name}_configindex = #{KeyCode.ConfigIndex(@name)};\n"
    if /^passthrough_/ =~ name then
      outfile << "static const bool remapclass_#{@name}_enable_when_passthrough = true;\n"
    else
      outfile << "static const bool remapclass_#{@name}_enable_when_passthrough = false;\n"
    end

    outfile << "static const unsigned int* remapclass_#{@name}_initialize_vector[] = {\n"
    outfile << "  BRIDGE_INITIALIZE_VECTOR_FORMAT_VERSION,\n"
    outfile << "  #{@code[:initialize_vector].count},\n"
    outfile << "  #{@code[:initialize_vector].join(',')}\n"
    outfile << "};\n"

    outfile << "static const char* remapclass_#{@name}_statusmessage = #{@code[:statusmessage]};\n"

    if @code[:setkeyboardtype].nil? then
      outfile << "static const bool remapclass_#{@name}_is_setkeyboardtype = false;\n"
      outfile << "static const unsigned int remapclass_#{@name}_setkeyboardtype = 0;\n"
    else
      outfile << "static const bool remapclass_#{@name}_is_setkeyboardtype = true;\n"
      outfile << "static const unsigned int remapclass_#{@name}_setkeyboardtype = #{@code[:setkeyboardtype]};\n"
    end

    outfile << "\n"
  end
end

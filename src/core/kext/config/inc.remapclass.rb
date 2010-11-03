#!/usr/bin/ruby
# -*- coding: utf-8 -*-

require 'inc.filter.rb'
require 'inc.keycode.rb'

class RemapClass
  @@simultaneous_keycode_index = 0
  @@variable_index = 0
  @@entries_notsave = []
  @@entries_normal = []

  def RemapClass.reset_variable_index
    @@variable_index = 0
  end

  def initialize(name)
    @name = name
    @initialize_vector = []

    if /^notsave_/ =~ name then
      @@entries_notsave << name
    else
      @@entries_normal << name
    end
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
        elsif /^KeyboardType::(.+)$/ =~ value then
          newdatatype = 'BRIDGE_DATATYPE_KEYBOARDTYPE'
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

    @initialize_vector << args.count
    @initialize_vector += args

    unless filtervec.empty? then
      @initialize_vector += filtervec
    end
  end
  protected :append_to_code_initialize_vector

  # ------------------------------------------------------------
  def preprocess_getextrakey(key)
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
  protected :preprocess_getextrakey

  def handle_autogen(autogen_text, filtervec)
    # ------------------------------------------------------------
    # preprocess
    case autogen_text
    when /VK_(COMMAND|CONTROL|SHIFT|OPTION)/ then
      key = $1
      handle_autogen(autogen_text.gsub(/VK_#{key}/, "ModifierFlag::#{key}_L"), filtervec)
      handle_autogen(autogen_text.gsub(/VK_#{key}/, "ModifierFlag::#{key}_R"), filtervec)
      return
    when /VK_MOD_CCOS_L/ then
      handle_autogen(autogen_text.gsub(/VK_MOD_CCOS_L/, "ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::OPTION_L | ModifierFlag::SHIFT_L"), filtervec)
      return
    when /VK_MOD_CCS_L/ then
      handle_autogen(autogen_text.gsub(/VK_MOD_CCS_L/, "ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L"), filtervec)
      return
    when /VK_MOD_CCO_L/ then
      handle_autogen(autogen_text.gsub(/VK_MOD_CCO_L/, "ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::OPTION_L"), filtervec)
      return
    when /VK_MOD_ANY/ then
      # to reduce combination, we ignore same modifier combination such as (COMMAND_L | COMMAND_R).
      modifiers = [
                   'VK_COMMAND',
                   'VK_CONTROL',
                   'ModifierFlag::FN',
                   'VK_OPTION',
                   'VK_SHIFT',
                  ]
      (0 .. modifiers.length).to_a.reverse.each do |i|
        modifiers.combination(i).to_a.each do |pattern|
          pat = pattern.dup
          pat << "ModifierFlag::NONE"
          handle_autogen(autogen_text.gsub(/VK_MOD_ANY/, pat.join(' | ')), filtervec)
        end
      end
      return
    when /FROMKEYCODE_(HOME|END|PAGEUP|PAGEDOWN|FORWARD_DELETE)\s*,\s*ModifierFlag::/ then
      key = $1
      extrakey = preprocess_getextrakey(key)
      handle_autogen(autogen_text.gsub(/FROMKEYCODE_#{key}\s*,/, "KeyCode::#{key},"), filtervec)
      handle_autogen(autogen_text.gsub(/FROMKEYCODE_#{key}\s*,/, "KeyCode::#{extrakey}, ModifierFlag::FN |"), filtervec)
      return
    when /FROMKEYCODE_(HOME|END|PAGEUP|PAGEDOWN|FORWARD_DELETE)/ then
      key = $1
      extrakey = preprocess_getextrakey(key)
      handle_autogen(autogen_text.gsub(/FROMKEYCODE_#{key}\s*,/, "KeyCode::#{key},"), filtervec)
      handle_autogen(autogen_text.gsub(/FROMKEYCODE_#{key}\s*,/, "KeyCode::#{extrakey}, ModifierFlag::FN,"), filtervec)
      return
    when /--KeyOverlaidModifierWithRepeat--/ then
      handle_autogen(autogen_text.gsub(/--KeyOverlaidModifierWithRepeat--/, '--KeyOverlaidModifier-- Option::KEYOVERLAIDMODIFIER_REPEAT, '), filtervec)
      return
    when /SimultaneousKeyPresses::Option::RAW/ then
      handle_autogen(autogen_text.gsub(/SimultaneousKeyPresses::Option::RAW/, 'Option::SIMULTANEOUSKEYPRESSES_RAW'), filtervec)
      return
    end

    # ------------------------------------------------------------
    # output
    if /^--(.+?)--(.*)/ =~ autogen_text then
      operation = $1
      params = $2

      case operation
      when 'ShowStatusMessage'
        @initialize_vector << params.length + 1
        @initialize_vector << 'BRIDGE_STATUSMESSAGE'
        params.each_byte do |byte|
          @initialize_vector << byte
        end
        # no need filtervec

      when 'SimultaneousKeyPresses'
        newkeycode = "VK_SIMULTANEOUSKEYPRESSES_#{@@simultaneous_keycode_index}"
        KeyCode.append_autoindexed('KeyCode', newkeycode)
        params = "KeyCode::#{newkeycode}, " + params
        @@simultaneous_keycode_index += 1
        append_to_code_initialize_vector(params, operation, filtervec)
        @@variable_index += 1

      when 'KeyToKey', 'KeyToConsumer', 'KeyToPointingButton', 'DoublePressModifier', 'HoldingKeyToKey', 'IgnoreMultipleSameKeyPress', 'KeyOverlaidModifier',
        'ConsumerToConsumer', 'ConsumerToKey', 'PointingButtonToPointingButton', 'PointingButtonToKey', 'PointingRelativeToScroll', 'DropKeyAfterRemap',
        'SetKeyboardType', 'DropPointingRelativeCursorMove'
        append_to_code_initialize_vector(params, operation, filtervec)
        @@variable_index += 1

      else
        print "%%% ERROR #{type} %%%\n#{l}\n"
        exit 1
      end
    end
  end

  def to_code(item_node, outfile)
    if item_node.find_first('./vk_config') then
      @initialize_vector << 5
      @initialize_vector << "BRIDGE_VK_CONFIG"
      @initialize_vector << KeyCode.v('KeyCode', "VK_CONFIG_TOGGLE_#{@name}")
      @initialize_vector << KeyCode.v('KeyCode', "VK_CONFIG_FORCE_ON_#{@name}")
      @initialize_vector << KeyCode.v('KeyCode', "VK_CONFIG_FORCE_OFF_#{@name}")
      @initialize_vector << KeyCode.v('KeyCode', "VK_CONFIG_SYNC_KEYDOWNUP_#{@name}")
    end

    item_node.find('.//autogen').each do |autogen_node|
      filter = Filter.new
      filtervec = filter.to_vector(autogen_node)
      unless /^passthrough_/ =~ @name then
        filtervec << 2
        filtervec << 'BRIDGE_FILTERTYPE_CONFIG_NOT'
        filtervec << KeyCode.ConfigIndex('notsave_passthrough')
      end

      autogen_text = autogen_node.inner_xml.gsub(/[\r\n]/, ' ').strip
      handle_autogen(autogen_text, filtervec)
    end

    # ----------------------------------------
    outfile << "static const unsigned int remapclass_#{@name}_configindex = #{KeyCode.ConfigIndex(@name)};\n"

    outfile << "static const unsigned int remapclass_#{@name}_initialize_vector[] = {\n"
    outfile << "  BRIDGE_REMAPCLASS_INITIALIZE_VECTOR_FORMAT_VERSION,\n"
    outfile << "  #{@initialize_vector.count},\n"
    outfile << "  #{@initialize_vector.join(',')}\n"
    outfile << "};\n"

    outfile << "\n"
  end

  def RemapClass.output_entries(outfile)
    [
     { :name => 'initialize_vector', :type => 'const unsigned int*' },
     { :name => 'configindex', :type => 'const unsigned int' },
    ].each do |info|
      outfile << "static #{info[:type]} remapclass_#{info[:name]}[] = {\n"
      [@@entries_notsave, @@entries_normal].each do |entries|
        entries.each do |name|
          outfile << "  remapclass_#{name}_#{info[:name]},\n"
        end
      end
      outfile << "};\n"
    end
  end
end

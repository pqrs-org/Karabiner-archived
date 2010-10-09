#!/usr/bin/ruby
# -*- coding: utf-8 -*-

require 'inc.filter.rb'

class RemapClass
  @@index = 0
  @@entries = []
  @@simultaneous_keycode_index = 0
  @@keycode = {}

  def RemapClass.get_entries
    return @@entries
  end

  def initialize(name)
    @name = name
    @filter = Filter.new()

    @code = {
      # functions
      :initialize                   => '',
      :terminate                    => '',
      :remap_setkeyboardtype        => '',
      :remap_key                    => '',
      :remap_consumer               => '',
      :remap_pointing               => '',
      :remap_simultaneouskeypresses => '',
      :remap_dropkeyafterremap      => '',
      :get_statusmessage            => '',
      # values
      :variable                     => [],
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

    load_keycode()
  end
  attr_accessor :name, :filter, :code

  def load_keycode
    return unless @@keycode.empty?

    Dir.glob("../keycode/output/*.raw").each do |filepath|
      type = File.basename(filepath, 'raw').gsub(/^include\./, '')
      open(filepath) do |f|
        while l = f.gets
          next unless /^(.+?) (.+)$/ =~ l
          @@keycode[$1] = $2
        end
      end
    end
  end

  def +(other)
    other.code.each do |k,v|
      @code[k] += v
    end
    self
  end

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
        if @@keycode[value].nil? then
          print "[ERROR] unknown keycode #{value}\n"
          throw :exit
        end
        newval << @@keycode[value]
      end
      args << datatype
      args << newval.join('|')
    end

    @code[:initialize] += "{\n"
    @code[:initialize] += "  const unsigned int vec[] = { #{args.join(',')} };\n"
    @code[:initialize] += "  value#{@@index}_.initialize(vec, sizeof(vec) / sizeof(vec[0]));\n"
    @code[:initialize] += "}\n"
  end
  protected :append_to_code_initialize

  def append_to_code_terminate
    @code[:terminate] += "static void terminate_value#{@@index}(void) {\n"
    @code[:terminate] += "value#{@@index}_.terminate();\n"
    @code[:terminate] += "}\n"

    @@entries[-1][:terminate] << "RemapClass_#{@name}::terminate_value#{@@index}"
  end

  # return true if 'line' contains autogen/filter definition.
  def parse(line)
    return true if @filter.parse(line)

    if /<autogen>--(.+?)-- (.+)<\/autogen>/ =~ line then
      operation = $1
      params = $2
      @@index += 1

      case operation
      when 'SetKeyboardType'
        @code[:remap_setkeyboardtype] += "keyboardType = #{params}.get();\n";

      when 'DropKeyAfterRemap'
        append_to_code_initialize(params, operation)
        append_to_code_terminate
        @code[:variable] << { :index => @@index, :class => "RemapFunc::#{operation}" }
        @code[:remap_dropkeyafterremap] += "if (value#{@@index}_.drop(params)) return true;\n"

      when 'ShowStatusMessage'
        @code[:get_statusmessage] += "return #{params};\n"

      when 'SimultaneousKeyPresses'
        params = "KeyCode::VK_SIMULTANEOUSKEYPRESSES_#{@@simultaneous_keycode_index}, " + params
        @@simultaneous_keycode_index += 1
        append_to_code_initialize(params, operation)
        append_to_code_terminate
        @code[:variable] << { :index => @@index, :class => "RemapFunc::SimultaneousKeyPresses" }
        @code[:remap_key] += "if (value#{@@index}_.remap(remapParams)) break;\n"
        @code[:remap_simultaneouskeypresses] += "value#{@@index}_.remap_SimultaneousKeyPresses();\n"

      when 'KeyToKey', 'KeyToConsumer', 'KeyToPointingButton', 'DoublePressModifier', 'HoldingKeyToKey', 'IgnoreMultipleSameKeyPress', 'KeyOverlaidModifier'
        append_to_code_initialize(params, operation)
        append_to_code_terminate
        @code[:variable] << { :index => @@index, :class => "RemapFunc::#{operation}" }
        @code[:remap_key] += "if (value#{@@index}_.remap(remapParams)) break;\n"

      when 'ConsumerToConsumer', 'ConsumerToKey'
        append_to_code_initialize(params, operation)
        append_to_code_terminate
        @code[:variable] << { :index => @@index, :class => "RemapFunc::#{operation}" }
        @code[:remap_consumer] += "if (value#{@@index}_.remap(remapParams)) break;\n"

      when 'PointingButtonToPointingButton', 'PointingButtonToKey', 'PointingRelativeToScroll'
        append_to_code_initialize(params, operation)
        append_to_code_terminate
        @code[:variable] << { :index => @@index, :class => "RemapFunc::#{operation}" }
        @code[:remap_pointing] += "if (value#{@@index}_.remap(remapParams)) break;\n"

      else
        print "%%% ERROR #{type} %%%\n#{l}\n"
        exit 1
      end

      return true
    end

    return false
  end

  def fixup
    [:remap_key, :remap_consumer,:remap_pointing, :remap_simultaneouskeypresses].each do |k|
      unless @code[k].empty? then
        c  = "do {\n"
        c += @filter.to_code
        c += @code[k]
        c += "} while (false);\n"
        @code[k] = c
      end
    end
  end

  def empty?
    @code.each do |k, v|
      return false unless v.empty?
    end
    return true
  end

  def to_code
    return '' if empty?

    classname = "RemapClass_#{@name}"

    code  = "class #{classname} {\n"
    code += "public:\n"

    # ----------------------------------------------------------------------
    code += "static void initialize(void) {\n"
    code += @code[:initialize]
    code += "}\n"
    @@entries[-1][:initialize] << "RemapClass_#{@name}::initialize"

    code += @code[:terminate]

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
    unless @code[:remap_key].empty? then
      code += "static void remap_key(RemapParams& remapParams) {\n"
      code += @code[:remap_key]
      code += "}\n"

      @@entries[-1][:remap_key] << "#{classname}::remap_key"
    else
      @@entries[-1][:remap_key] << "NULL"
    end

    # ----------------------------------------------------------------------
    unless @code[:remap_consumer].empty? then
      code += "static void remap_consumer(RemapConsumerParams& remapParams) {\n"
      code += @code[:remap_consumer]
      code += "}\n"

      @@entries[-1][:remap_consumer] << "#{classname}::remap_consumer"
    else
      @@entries[-1][:remap_consumer] << "NULL"
    end

    # ----------------------------------------------------------------------
    unless @code[:remap_pointing].empty? then
      code += "static void remap_pointing(RemapPointingParams_relative& remapParams) {\n"
      code += @code[:remap_pointing]
      code += "}\n"

      @@entries[-1][:remap_pointing] << "#{classname}::remap_pointing"
    else
      @@entries[-1][:remap_pointing] << "NULL"
    end

    # ----------------------------------------------------------------------
    unless @code[:remap_simultaneouskeypresses].empty? then
      code += "static void remap_simultaneouskeypresses(void) {\n"
      code += @code[:remap_simultaneouskeypresses]
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
    unless @code[:remap_dropkeyafterremap].empty? then
      code += "static bool remap_dropkeyafterremap(const Params_KeyboardEventCallBack& params) {\n"
      code += "if (! enabled()) return false;\n"
      code += @code[:remap_dropkeyafterremap]
      code += "return false;\n"
      code += "}\n"

      @@entries[-1][:remap_dropkeyafterremap] << "#{classname}::remap_dropkeyafterremap"
    end

    # ----------------------------------------------------------------------
    code   += "static bool enabled(void) {\n"
    if /^passthrough_/ =~ @name or @name == 'notsave_passthrough' then
      code += "return config.#{@name};\n"
    else
      code += "return config.#{@name} && ! config.notsave_passthrough;\n"
    end
    code   += "}\n"

    @@entries[-1][:enabled] << "#{classname}::enabled"

    # ----------------------------------------
    code += "\n"
    code += "private:\n"
    @code[:variable].each do |v|
      code += "static RemapClass::Item value#{v[:index]}_;\n"
    end
    code += "};\n"

    @code[:variable].each do |v|
      code += "RemapClass::Item #{classname}::value#{v[:index]}_;\n"
    end
    code += "\n\n"

    code
  end
end

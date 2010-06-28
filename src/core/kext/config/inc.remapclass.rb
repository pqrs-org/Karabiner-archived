#!/usr/bin/ruby
# -*- coding: utf-8 -*-

require 'inc.filter.rb'

class RemapClass
  @@index = 0
  @@entries = {
    :initialize                   => [],
    :terminate                    => [],
    :handlevirtualkey             => [],
    :remap_setkeyboardtype        => [],
    :remap_key                    => [],
    :remap_consumer               => [],
    :remap_pointing               => [],
    :remap_simultaneouskeypresses => [],
    :get_statusmessage            => [],
    :enabled                      => [],
  }

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
      :handlevirtualkey             => '',
      :remap_setkeyboardtype        => '',
      :remap_key                    => '',
      :remap_consumer               => '',
      :remap_pointing               => '',
      :remap_simultaneouskeypresses => '',
      :get_statusmessage            => '',
      # values
      :keycode                      => '',
      :variable                     => [],
    }
  end
  attr_accessor :name, :filter, :code

  def +(other)
    other.code.each do |k,v|
      @code[k] += v
    end
    self
  end

  def append_to_code_initialize(params)
    # We split the initialize function per value.
    # If a large number of values exist, the kernel stack is wasted when the monolithic initialize function is called.
    # So, we split it.
    @code[:initialize] += "static void initialize_value#{@@index}(void) {\n"
    @code[:initialize] += "value#{@@index}_.initialize();\n"
    params.split(/,/).each do |p|
      @code[:initialize] += "value#{@@index}_.add(#{p.strip});\n"
    end
    @code[:initialize] += "}\n"

    @@entries[:initialize] << "RemapClass_#{@name}::initialize_value#{@@index}"
  end
  protected :append_to_code_initialize

  def append_to_code_terminate
    @code[:terminate] += "static void terminate_value#{@@index}(void) {\n"
    @code[:terminate] += "value#{@@index}_.terminate();\n"
    @code[:terminate] += "}\n"

    @@entries[:terminate] << "RemapClass_#{@name}::terminate_value#{@@index}"
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

      when 'ShowStatusMessage'
        @code[:get_statusmessage] += "return #{params};\n"

      when 'SimultaneousKeyPresses'
        @code[:variable] << { :index => @@index, :class => "EventInputQueue::Remap" }
        @code[:keycode] += "VK_SIMULTANEOUSKEYPRESSES_#{name}_#{@@index} --AUTO--\n"
        @code[:remap_simultaneouskeypresses] += "value#{@@index}_.remap();\n"
        @code[:handlevirtualkey] += "if (value#{@@index}_.handleVirtualKey(params)) return true;\n"

        @code[:initialize] += "static void initialize_value#{@@index}(void) {\n"
        @code[:initialize] += "value#{@@index}_.initialize(KeyCode::VK_SIMULTANEOUSKEYPRESSES_#{name}_#{@@index}, #{params});\n"
        @code[:initialize] += "}\n"
        @@entries[:initialize] << "RemapClass_#{@name}::initialize_value#{@@index}"

      when 'KeyToKey', 'KeyToConsumer', 'IgnoreMultipleSameKeyPress'
        append_to_code_initialize(params)
        append_to_code_terminate
        @code[:variable] << { :index => @@index, :class => "RemapFunc::#{operation}" }
        @code[:remap_key] += "if (value#{@@index}_.remap(remapParams)) break;\n"

      when 'ConsumerToConsumer'
        append_to_code_initialize(params)
        append_to_code_terminate
        @code[:variable] << { :index => @@index, :class => "RemapFunc::ConsumerToConsumer" }
        @code[:remap_consumer] += "if (value#{@@index}_.remap(remapParams)) break;\n"

      when 'DoublePressModifier'
        @code[:variable] << { :index => @@index, :class => "DoublePressModifier" }
        @code[:remap_key] += "if (value#{@@index}_.remap(remapParams, #{params})) break;\n"

      when 'KeyToPointingButton'
        @code[:variable] << { :index => @@index, :class => "RemapUtil::KeyToPointingButton" }
        @code[:remap_key] += "if (value#{@@index}_.remap(remapParams, #{params})) break;\n"

      when 'KeyOverlaidModifier'
        @code[:variable] << { :index => @@index, :class => "KeyOverlaidModifier" }
        @code[:remap_key] += "if (value#{@@index}_.remap(remapParams, #{params})) break;\n"

      when 'KeyOverlaidModifierWithRepeat'
        @code[:variable] << { :index => @@index, :class => "KeyOverlaidModifier" }
        @code[:remap_key] += "if (value#{@@index}_.remapWithRepeat(remapParams, #{params})) break;\n"

      when 'ModifierHoldingKeyToKey'
        @code[:variable] << { :index => @@index, :class => "ModifierHoldingKeyToKey" }
        @code[:remap_key] += "if (value#{@@index}_.remap(remapParams, #{params})) break;\n"

      when 'HoldingKeyToKey'
        @code[:variable] << { :index => @@index, :class => "HoldingKeyToKey" }
        @code[:remap_key] += "if (value#{@@index}_.remap(remapParams, #{params})) break;\n"

      when 'ConsumerToKey'
        append_to_code_initialize(params)
        append_to_code_terminate
        @code[:variable] << { :index => @@index, :class => "RemapUtil::ConsumerToKey" }
        @code[:remap_consumer] += "if (value#{@@index}_.remap(remapParams)) break;\n"

      when 'PointingRelativeToScroll'
        @code[:variable] << { :index => @@index, :class => "RemapUtil::PointingRelativeToScroll" }
        @code[:remap_pointing] += "if (value#{@@index}_.remap(remapParams, #{params})) break;\n"

      when 'PointingButtonToPointingButton'
        @code[:variable] << { :index => @@index, :class => "RemapUtil::PointingButtonToPointingButton" }
        @code[:remap_pointing] += "if (value#{@@index}_.remap(remapParams, #{params})) break;\n"

      when 'PointingButtonToKey'
        @code[:variable] << { :index => @@index, :class => "RemapUtil::PointingButtonToKey" }
        @code[:remap_pointing] += "if (value#{@@index}_.remap(remapParams, #{params})) break;\n"

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
    code += @code[:initialize]
    code += @code[:terminate]

    # ----------------------------------------------------------------------
    unless @code[:remap_setkeyboardtype].empty? then
      code += "static void remap_setkeyboardtype(KeyboardType& keyboardType) {\n"
      code += @code[:remap_setkeyboardtype]
      code += "}\n"

      @@entries[:remap_setkeyboardtype] << "#{classname}::remap_setkeyboardtype"
    else
      @@entries[:remap_setkeyboardtype] << "NULL"
    end

    # ----------------------------------------------------------------------
    unless @code[:remap_key].empty? then
      code += "static void remap_key(RemapParams& remapParams) {\n"
      code += @code[:remap_key]
      code += "}\n"

      @@entries[:remap_key] << "#{classname}::remap_key"
    else
      @@entries[:remap_key] << "NULL"
    end

    # ----------------------------------------------------------------------
    unless @code[:remap_consumer].empty? then
      code += "static void remap_consumer(RemapConsumerParams& remapParams) {\n"
      code += @code[:remap_consumer]
      code += "}\n"

      @@entries[:remap_consumer] << "#{classname}::remap_consumer"
    else
      @@entries[:remap_consumer] << "NULL"
    end

    # ----------------------------------------------------------------------
    unless @code[:remap_pointing].empty? then
      code += "static void remap_pointing(RemapPointingParams_relative& remapParams) {\n"
      code += @code[:remap_pointing]
      code += "}\n"

      @@entries[:remap_pointing] << "#{classname}::remap_pointing"
    else
      @@entries[:remap_pointing] << "NULL"
    end

    # ----------------------------------------------------------------------
    unless @code[:remap_simultaneouskeypresses].empty? then
      code += "static void remap_simultaneouskeypresses(void) {\n"
      code += @code[:remap_simultaneouskeypresses]
      code += "}\n"

      @@entries[:remap_simultaneouskeypresses] << "#{classname}::remap_simultaneouskeypresses"
    else
      @@entries[:remap_simultaneouskeypresses] << "NULL"
    end

    # ----------------------------------------------------------------------
    unless @code[:get_statusmessage].empty? then
      code += "static const char* get_statusmessage(void) {\n"
      code += @code[:get_statusmessage]
      code += "}\n"

      @@entries[:get_statusmessage] << "#{classname}::get_statusmessage"
    else
      @@entries[:get_statusmessage] << "NULL"
    end

    # ----------------------------------------------------------------------
    unless @code[:handlevirtualkey].empty? then
      code += "static bool handlevirtualkey(const Params_KeyboardEventCallBack& params) {\n"
      code += @code[:handlevirtualkey]
      code += "return false;\n"
      code += "}\n"

      @@entries[:handlevirtualkey] << "#{classname}::handlevirtualkey"
    end

    # ----------------------------------------
    code   += "static bool enabled(void) {\n"
    if /^passthrough_/ =~ @name or @name == 'notsave_passthrough' then
      code += "return config.#{@name};\n"
    else
      code += "return config.#{@name} && ! config.notsave_passthrough;\n"
    end
    code   += "}\n"

    @@entries[:enabled] << "#{classname}::enabled"

    # ----------------------------------------
    code += "\n"
    code += "private:\n"
    @code[:variable].each do |v|
      code += "static #{v[:class]} value#{v[:index]}_;\n"
    end
    code += "};\n"

    @code[:variable].each do |v|
      code += "#{v[:class]} #{classname}::value#{v[:index]}_;\n"
    end
    code += "\n\n"

    code
  end
end

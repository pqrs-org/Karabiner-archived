#!/usr/bin/ruby
# -*- coding: utf-8 -*-

require 'inc.filter.rb'

class RemapClass
  @@index = 0

  def initialize(name, autogen_index)
    @name = name
    @filter = Filter.new()

    @code = {
      :initialize                      => '',
      :remap_key                       => '',
      :remap_consumer                  => '',
      :remap_pointing                  => '',
      :remap_setkeyboardtype           => '',
      :remap_simultaneouskeypresses    => '',
      :keycode                         => '',
      :statusmessage                   => '',

      :variable                        => [],
      :simultaneouskeypresses_variable => [],
    }

    @autogen_index = autogen_index
  end
  attr_accessor :name, :filter, :code, :autogen_index

  def +(other)
    other.code.each do |k,v|
      @code[k] += v
    end
    self
  end

  # return true if 'line' contains autogen/filter definition.
  def parse(line)
    return true if @filter.parse(line)

    if /<autogen>--(.+?)-- (.+)<\/autogen>/ =~ line then
      operation = $1
      params = $2
      @autogen_index += 1

      case operation
      when 'SetKeyboardType'
        @code[:remap_setkeyboardtype] += "if (config.#{name} && ! config.notsave_passthrough) {\n"
        @code[:remap_setkeyboardtype] += "  keyboardType = #{params}.get();\n"
        @code[:remap_setkeyboardtype] += "}\n"

      when 'ShowStatusMessage'
        @code[:statusmessage] = "#{params};\n"

      when 'SimultaneousKeyPresses'
        @code[:keycode] += "VK_SIMULTANEOUSKEYPRESSES_#{name}_#{@autogen_index} --AUTO--\n"
        $func[:simultaneouskeypresses] << name
        @code[:simultaneouskeypresses_variable] << { :name => "remap_#{@autogen_index}_", :params => "KeyCode::VK_SIMULTANEOUSKEYPRESSES_#{name}_#{@autogen_index}, #{params}" }
        @code[:remap_simultaneouskeypresses] += "remap_#{@autogen_index}_.remap();\n"

      when 'KeyToKey'
        @code[:variable] << ['RemapUtil::KeyToKey', "keytokey#{@autogen_index}_"]
        @code[:remap_key] += "    if (keytokey#{@autogen_index}_.remap(remapParams, #{params})) break;\n"

      when 'DoublePressModifier'
        @code[:variable] << ['DoublePressModifier', "doublepressmodifier#{@autogen_index}_"]
        @code[:remap_key] += "    if (doublepressmodifier#{@autogen_index}_.remap(remapParams, #{params})) break;\n"

      when 'IgnoreMultipleSameKeyPress'
        @code[:variable] << ['IgnoreMultipleSameKeyPress', "ignoremultiplesamekeypress#{@autogen_index}_"]
        @code[:remap_key] += "    if (ignoremultiplesamekeypress#{@autogen_index}_.remap(remapParams, #{params})) break;\n"

      when 'KeyToConsumer'
        @code[:variable] << ['RemapUtil::KeyToConsumer', "keytoconsumer#{@autogen_index}_"]
        @code[:remap_key] += "    if (keytoconsumer#{@autogen_index}_.remap(remapParams, #{params})) break;\n"

      when 'KeyToPointingButton'
        @code[:variable] << ['RemapUtil::KeyToPointingButton', "keytopointing#{@autogen_index}_"]
        @code[:remap_key] += "    if (keytopointing#{@autogen_index}_.remap(remapParams, #{params})) break;\n"

      when 'KeyOverlaidModifier'
        @code[:variable] << ['KeyOverlaidModifier', "keyoverlaidmodifier#{@autogen_index}_"]
        @code[:remap_key] += "    if (keyoverlaidmodifier#{@autogen_index}_.remap(remapParams, #{params})) break;\n"

      when 'KeyOverlaidModifierWithRepeat'
        @code[:variable] << ['KeyOverlaidModifier', "keyoverlaidmodifier#{@autogen_index}_"]
        @code[:remap_key] += "    if (keyoverlaidmodifier#{@autogen_index}_.remapWithRepeat(remapParams, #{params})) break;\n"

      when 'ModifierHoldingKeyToKey'
        @code[:variable] << ['ModifierHoldingKeyToKey', "modifierholdingkeytokey#{@autogen_index}_"]
        @code[:remap_key] += "    if (modifierholdingkeytokey#{@autogen_index}_.remap(remapParams, #{params})) break;\n"

      when 'HoldingKeyToKey'
        @code[:variable] << ['HoldingKeyToKey', "holdingkeytokey#{@autogen_index}_"]
        @code[:remap_key] += "    if (holdingkeytokey#{@autogen_index}_.remap(remapParams, #{params})) break;\n"

      when 'ConsumerToKey'
        @code[:variable] << ['RemapUtil::ConsumerToKey', "consumertokey#{@autogen_index}_"]
        @code[:remap_consumer] += "    if (consumertokey#{@autogen_index}_.remap(remapParams, #{params})) break;\n"

      when 'ConsumerToConsumer'
        @code[:variable] << ['RemapUtil::ConsumerToConsumer', "consumertoconsumer#{@autogen_index}_"]
        @code[:remap_consumer] += "    if (consumertoconsumer#{@autogen_index}_.remap(remapParams, #{params})) break;\n"

      when 'PointingRelativeToScroll'
        @code[:variable] << ['RemapUtil::PointingRelativeToScroll', "pointingrelativetoscroll#{@autogen_index}_"]
        @code[:remap_pointing] += "    if (pointingrelativetoscroll#{@autogen_index}_.remap(remapParams, #{params})) break;\n"

      when 'PointingButtonToPointingButton'
        @code[:variable] << ['RemapUtil::PointingButtonToPointingButton', "pointingbuttontopointingbutton#{@autogen_index}_"]
        @code[:remap_pointing] << "    if (pointingbuttontopointingbutton#{@autogen_index}_.remap(remapParams, #{params})) break;\n"

      when 'PointingButtonToKey'
        @code[:variable] << ['RemapUtil::PointingButtonToKey', "pointingbuttontokey#{@autogen_index}_"]
        @code[:remap_pointing] << "    if (pointingbuttontokey#{@autogen_index}_.remap(remapParams, #{params})) break;\n"

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
      next if k == :remap_setkeyboardtype
      return false unless v.empty?
    end
    return true
  end

  def to_code
    return '' if empty?

    code  = "class RemapClass_#{@name} : public RemapClass {\n"
    code += "public:\n"

    code += "void initialize(void) {\n"
    code += @code[:initialize]
    unless @code[:statusmessage].empty? then
      code += "statusmessage = #{@code[:statusmessage]}"
    end
    code += "}\n"

    unless @code[:remap_key].empty? then
      code += "void remap_key(RemapParams &remapParams) {\n"
      code += @code[:remap_key]
      code += "}\n"
    end
    unless @code[:remap_consumer].empty? then
      code += "void remap_consumer(RemapConsumerParams &remapParams) {\n"
      code += @code[:remap_consumer]
      code += "}\n"
    end
    unless @code[:remap_pointing].empty? then
      code += "void remap_pointing(RemapPointingParams_relative &remapParams) {\n"
      code += @code[:remap_pointing]
      code += "}\n"
    end

    # ----------------------------------------
    code   += "bool enabled(EnableType type) const {\n"
    if @code[:remap_setkeyboardtype].empty? then
      code += "if (type == ENABLE_TYPE_SETKEYBOARDTYPE) return false;\n"
    end
    if @code[:remap_key].empty? then
      code += "if (type == ENABLE_TYPE_KEY) return false;\n"
    end
    if @code[:remap_consumer].empty? then
      code += "if (type == ENABLE_TYPE_CONSUMER) return false;\n"
    end
    if @code[:remap_pointing].empty? then
      code += "if (type == ENABLE_TYPE_POINTING) return false;\n"
    end
    if @code[:statusmessage].empty? then
      code += "if (type == ENABLE_TYPE_STATUSMESSAGE) return false;\n"
    end

    if /^passthrough_/ =~ @name or @name == 'notsave_passthrough' then
      code += "return config.#{@name};\n"
    else
      code += "return config.#{@name} && ! config.notsave_passthrough;\n"
    end
    code   += "}\n"

    # ----------------------------------------
    code += "\n"
    code += "private:\n"
    @code[:variable].each do |variable|
      code += "  #{variable[0]} #{variable[1]};\n"
    end
    code += "};\n"

    code += "RemapClass_#{@name} remapclass_#{@name};\n"
    code += "\n\n"

    code
  end
end

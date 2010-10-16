#!/usr/bin/ruby
# -*- coding: utf-8 -*-

require 'inc.keycode.rb'

class Filter
  def initialize
    @array = []
    @filters = []
  end

  # return true if 'line' contains filter definition.
  def parse(line)
    if /<not>(.+?)<\/not>/ =~ line then
      vec = []
      vec << 'BRIDGE_FILTERTYPE_APPLICATION_NOT'
      $1.split(/,/).each do |f|
        @array << "if (CommonData::getcurrent_workspacedata().type == KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}) break;"
        vec << "KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}"
      end
      @filters << vec
      return true
    end

    if /<only>(.+?)<\/only>/ =~ line then
      vec = []
      vec << 'BRIDGE_FILTERTYPE_APPLICATION_ONLY'

      tmp = []
      $1.split(/,/).each do |f|
        tmp << "(CommonData::getcurrent_workspacedata().type != KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip})"
        vec << "KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}"
      end
      @array << "if (#{tmp.join(' && ')}) break;"
      @filters << vec
      return true
    end

    if /<device_not>(.+?)<\/device_not>/ =~ line then
      vec = []
      vec << 'BRIDGE_FILTERTYPE_DEVICE_NOT'

      $1.scan(/DeviceVendorID\(.+?\)\s*,\s*DeviceProductID\(.+?\)/).each do |f|
        @array << "if (CommonData::isEqualVendorIDProductID(#{f.strip})) break;"
        vec += f.strip.split(/,/)
      end
      @filters << vec
      return true
    end

    if /<device_only>(.+?)<\/device_only>/ =~ line then
      vec = []
      vec << 'BRIDGE_FILTERTYPE_DEVICE_ONLY'

      tmp = []
      $1.scan(/DeviceVendorID\(.+?\)\s*,\s*DeviceProductID\(.+?\)/).each do |f|
        tmp << "(! CommonData::isEqualVendorIDProductID(#{f.strip}))"
        vec += f.strip.split(/,/)
      end
      @array << "if (#{tmp.join(' && ')}) break;"
      @filters << vec
      return true
    end

    if /<config_not>(.+?)<\/config_not>/ =~ line then
      vec = []
      vec << 'BRIDGE_FILTERTYPE_CONFIG_NOT'

      $1.split(/,/).each do |f|
        f.gsub!(/\./, '_')
        @array << "if (config.enabled_flags[#{KeyCode.ConfigIndex(f.strip)}]) break;"

        vec << KeyCode.ConfigIndex(f.strip)
      end
      @filters << vec
      return true
    end

    if /<config_only>(.+?)<\/config_only>/ =~ line then
      vec = []
      vec << 'BRIDGE_FILTERTYPE_CONFIG_ONLY'

      tmp = []
      $1.split(/,/).each do |f|
        f.gsub!(/\./, '_')
        tmp << "(! config.enabled_flags[#{KeyCode.ConfigIndex(f.strip)}])"

        vec << KeyCode.ConfigIndex(f.strip)
      end
      @array << "if (#{tmp.join(' && ')}) break;"
      @filters << vec
      return true
    end

    if /<(inputmode|inputmodedetail)_not>(.+?)<\/(inputmode|inputmodedetail)_not>/ =~ line then
      inputmodetype = $1
      body = $2

      vec = []
      if /<inputmode_not>/ =~ line then
        vec << 'BRIDGE_FILTERTYPE_INPUTMODE_NOT'
      else
        vec << 'BRIDGE_FILTERTYPE_INPUTMODEDETAIL_NOT'
      end

      body.split(/,/).each do |f|
        @array << "if (CommonData::getcurrent_workspacedata().#{inputmodetype} == KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}) break;"
        vec << "KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}"
      end
      @filters << vec
      return true
    end

    if /<(inputmode|inputmodedetail)_only>(.+?)<\/(inputmode|inputmodedetail)_only>/ =~ line then
      inputmodetype = $1
      body = $2

      vec = []
      if /<inputmode_only>/ =~ line then
        vec << 'BRIDGE_FILTERTYPE_INPUTMODE_ONLY'
      else
        vec << 'BRIDGE_FILTERTYPE_INPUTMODEDETAIL_ONLY'
      end

      tmp = []
      body.split(/,/).each do |f|
        tmp << "(CommonData::getcurrent_workspacedata().#{inputmodetype} != KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip})"
        vec << "KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}"
      end
      @array << "if (#{tmp.join(' && ')}) break;"
      @filters << vec
      return true
    end
  end

  def to_code
    code = ''
    @array.each do |f|
      code += "    #{f}\n"
    end
    code += "\n" unless code.empty?
    code
  end
end

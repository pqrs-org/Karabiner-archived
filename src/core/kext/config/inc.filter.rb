#!/usr/bin/ruby
# -*- coding: utf-8 -*-

require 'inc.keycode.rb'

class Filter
  def initialize
    @array = []
  end

  # return true if 'line' contains filter definition.
  def parse(line)
    if /<not>(.+?)<\/not>/ =~ line then
      $1.split(/,/).each do |f|
        @array << "if (CommonData::getcurrent_workspacedata().type == KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}) break;"
      end
      return true
    end

    if /<only>(.+?)<\/only>/ =~ line then
      tmp = []
      $1.split(/,/).each do |f|
        tmp << "(CommonData::getcurrent_workspacedata().type != KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip})"
      end
      @array << "if (#{tmp.join(' && ')}) break;"
      return true
    end

    if /<device_not>(.+?)<\/device_not>/ =~ line then
      $1.scan(/DeviceVendorID\(.+?\)\s*,\s*DeviceProductID\(.+?\)/).each do |f|
        @array << "if (CommonData::isEqualVendorIDProductID(#{f.strip})) break;"
      end
      return true
    end

    if /<device_only>(.+?)<\/device_only>/ =~ line then
      tmp = []
      $1.scan(/DeviceVendorID\(.+?\)\s*,\s*DeviceProductID\(.+?\)/).each do |f|
        tmp << "(! CommonData::isEqualVendorIDProductID(#{f.strip}))"
      end
      @array << "if (#{tmp.join(' && ')}) break;"
      return true
    end

    if /<config_not>(.+?)<\/config_not>/ =~ line then
      $1.split(/,/).each do |f|
        f.gsub!(/\./, '_')
        @array << "if (config.enabled_flags[#{KeyCode.ConfigIndex(f.strip)}]) break;"
      end
      return true
    end

    if /<config_only>(.+?)<\/config_only>/ =~ line then
      tmp = []
      $1.split(/,/).each do |f|
        f.gsub!(/\./, '_')
        tmp << "(! config.enabled_flags[#{KeyCode.ConfigIndex(f.strip)}])"
      end
      @array << "if (#{tmp.join(' && ')}) break;"
      return true
    end

    if /<(inputmode|inputmodedetail)_not>(.+?)<\/(inputmode|inputmodedetail)_not>/ =~ line then
      inputmodetype = $1
      $2.split(/,/).each do |f|
        @array << "if (CommonData::getcurrent_workspacedata().#{inputmodetype} == KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}) break;"
      end
      return true
    end

    if /<(inputmode|inputmodedetail)_only>(.+?)<\/(inputmode|inputmodedetail)_only>/ =~ line then
      inputmodetype = $1
      tmp = []
      $2.split(/,/).each do |f|
        tmp << "(CommonData::getcurrent_workspacedata().#{inputmodetype} != KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip})"
      end
      @array << "if (#{tmp.join(' && ')}) break;"
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

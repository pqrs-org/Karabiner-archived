#!/usr/bin/ruby
# -*- coding: utf-8 -*-

require 'inc.keycode.rb'

class Filter
  def initialize
    @filters = []
  end

  def append_application(filtertype, node)
    vec = [filtertype]

    node.inner_xml.split(/,/).each do |f|
      vec << "KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}"
    end

    @filters << vec.count
    @filters += vec
  end

  def append_device(filtertype, node)
    vec = [filtertype]

    node.inner_xml.scan(/DeviceVendorID\(.+?\)\s*,\s*DeviceProductID\(.+?\)/).each do |f|
      vec += f.strip.split(/,/)
    end

    @filters << vec.count
    @filters += vec
  end

  def append_config(filtertype, node)
    vec = [filtertype]

    node.inner_xml.split(/,/).each do |f|
      vec << KeyCode.ConfigIndex(f.strip.gsub(/\./, '_'))
    end

    @filters << vec.count
    @filters += vec
  end

  def append_inputmode(filtertype, node)
    vec = [filtertype]

    node.inner_xml.split(/,/).each do |f|
      vec << "KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}"
    end

    @filters << vec.count
    @filters += vec
  end

  def to_vector(autogen_node)
    while autogen_node.name != 'item'
      autogen_node.parent.children.each do |node|
        case node.name
        when 'not' then
          append_application('BRIDGE_FILTERTYPE_APPLICATION_NOT', node)
        when 'only' then
          append_application('BRIDGE_FILTERTYPE_APPLICATION_ONLY', node)

        when 'device_not' then
          append_device('BRIDGE_FILTERTYPE_DEVICE_NOT', node)
        when 'device_only' then
          append_device('BRIDGE_FILTERTYPE_DEVICE_ONLY', node)

        when 'config_not' then
          append_config('BRIDGE_FILTERTYPE_CONFIG_NOT', node)
        when 'config_only' then
          append_config('BRIDGE_FILTERTYPE_CONFIG_ONLY', node)

        when 'inputmode_not' then
          append_inputmode('BRIDGE_FILTERTYPE_INPUTMODE_NOT', node)
        when 'inputmode_only' then
          append_inputmode('BRIDGE_FILTERTYPE_INPUTMODE_ONLY', node)

        when 'inputmodedetail_not'
          append_inputmode('BRIDGE_FILTERTYPE_INPUTMODEDETAIL_NOT', node)
        when 'inputmodedetail_only'
          append_inputmode('BRIDGE_FILTERTYPE_INPUTMODEDETAIL_ONLY', node)
        end
      end

      autogen_node = autogen_node.parent
    end

    @filters
  end
end

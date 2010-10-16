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

    @filters << vec
  end

  def append_device(filtertype, node)
    vec = [filtertype]

    node.inner_xml.scan(/DeviceVendorID\(.+?\)\s*,\s*DeviceProductID\(.+?\)/).each do |f|
      vec += f.strip.split(/,/)
    end

    @filters << vec
  end

  def append_config(filtertype, node)
    vec = [filtertype]

    node.inner_xml.split(/,/).each do |f|
      vec << KeyCode.ConfigIndex(f.strip.gsub(/\./, '_'))
    end

    @filters << vec
  end

  def append_inputmode(filtertype, node)
    vec = [filtertype]

    node.inner_xml.split(/,/).each do |f|
      vec << "KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}"
    end

    @filters << vec
  end

  def to_code(item_node, autogen_node)
    while autogen_node != item_node
      # ----------------------------------------
      autogen_node.parent.find('./not').each do |node|
        append_application('BRIDGE_FILTERTYPE_APPLICATION_NOT', node)
      end

      autogen_node.parent.find('./only').each do |node|
        append_application('BRIDGE_FILTERTYPE_APPLICATION_ONLY', node)
      end

      # ----------------------------------------
      autogen_node.parent.find('./device_not').each do |node|
        append_device('BRIDGE_FILTERTYPE_DEVICE_NOT', node)
      end

      autogen_node.parent.find('./device_only').each do |node|
        append_device('BRIDGE_FILTERTYPE_DEVICE_ONLY', node)
      end

      # ----------------------------------------
      autogen_node.parent.find('./config_not').each do |node|
        append_config('BRIDGE_FILTERTYPE_CONFIG_NOT', node)
      end

      autogen_node.parent.find('./config_only').each do |node|
        append_config('BRIDGE_FILTERTYPE_CONFIG_ONLY', node)
      end

      # ----------------------------------------
      autogen_node.parent.find('./inputmode_not').each do |node|
        append_inputmode('BRIDGE_FILTERTYPE_INPUTMODE_NOT', node)
      end

      autogen_node.parent.find('./inputmode_only').each do |node|
        append_inputmode('BRIDGE_FILTERTYPE_INPUTMODE_ONLY', node)
      end

      autogen_node.parent.find('./inputmodedetail_not').each do |node|
        append_inputmode('BRIDGE_FILTERTYPE_INPUTMODEDETAIL_NOT', node)
      end

      autogen_node.parent.find('./inputmodedetail_only').each do |node|
        append_inputmode('BRIDGE_FILTERTYPE_INPUTMODEDETAIL_ONLY', node)
      end

      autogen_node = autogen_node.parent
    end

    vec = []
    vec << @filters.count

    @filters.each do |f|
      vec << f.count
      vec += f
    end

    vec.join(',')
  end
end

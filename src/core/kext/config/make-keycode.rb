#!/usr/bin/ruby
# -*- coding: utf-8 -*-

$outfile = {
  :keycode_vk_config => open('../keycode/data/include/KeyCode.VK_GENERATED.data', 'w'),
}

# ======================================================================
require 'rubygems'
require 'xml/libxml'
require 'inc.preprocess.rb'

parser = XML::Parser.file('../../../../files/prefpane/output/checkbox.xml')
libxmldoc = parser.parse

# ----------------------------------------
libxmldoc.root.find('//vk_config').each do |node|
  list = node.parent.find('./sysctl')
  throw :exit if list.length != 1
  name = list[0].inner_xml.gsub(/\./, '_')

  $outfile[:keycode_vk_config] << "VK_CONFIG_TOGGLE_#{name} --AUTO--\n"
  $outfile[:keycode_vk_config] << "VK_CONFIG_FORCE_ON_#{name} --AUTO--\n"
  $outfile[:keycode_vk_config] << "VK_CONFIG_FORCE_OFF_#{name} --AUTO--\n"
  $outfile[:keycode_vk_config] << "VK_CONFIG_SYNC_KEYDOWNUP_#{name} --AUTO--\n"
end

# ----------------------------------------
index = 0
libxmldoc.root.find('//autogen').each do |node|
  text = node.inner_xml
  Preprocesser.new().preprocess([text]).each do |text|
    if /--SimultaneousKeyPresses--/ =~ text then
      $outfile[:keycode_vk_config] << "VK_SIMULTANEOUSKEYPRESSES_#{index} --AUTO--\n"
      index += 1
    end
  end
end

# ======================================================================
$outfile.each do |name,file|
  file.close
end

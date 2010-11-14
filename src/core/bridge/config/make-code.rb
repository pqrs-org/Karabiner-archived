#!/usr/bin/ruby

require 'rubygems'
require 'xml/libxml'

GC.disable

$outfile = {
  :hpp   => open('output/include.bridge_essential_config_index.hpp', 'w'),
  :cpp   => open('output/include.bridge_essential_config_index.cpp', 'w'),
  :plist => open('output/include.bridge_essential_config_index.plist', 'w'),
}

configindex = 0

$outfile[:plist].print '<?xml version="1.0" encoding="UTF-8"?>' + "\n"
$outfile[:plist].print '<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">' + "\n"
$outfile[:plist].print '<plist version="1.0">' + "\n"
$outfile[:plist].print '  <array>' + "\n"

ARGV.each do |xmlpath|
  parser = XML::Parser.file(xmlpath)
  libxmldoc = parser.parse

  libxmldoc.root.find('//sysctl').each do |sysctl_node|
    if sysctl_node['essential'] == 'true' then
      name = sysctl_node.inner_xml
      sysctl_entry = name.split('.')[0]
      sysctl_name  = name.split('.')[1]
      name.gsub!(/\./, '_')

      default_node = sysctl_node.parent.find_first('./default')
      if default_node.nil? then
        $outfile[:cpp].print "0,\n"
      else
        $outfile[:cpp].print "#{default_node.inner_xml},\n"
      end

      $outfile[:hpp].print "BRIDGE_ESSENTIAL_CONFIG_INDEX_#{name} = #{configindex},\n"
      $outfile[:plist].print "    <string>#{name}</string>\n"
      configindex += 1
    end
  end
end

$outfile[:hpp].print "BRIDGE_ESSENTIAL_CONFIG_INDEX__END__ = #{configindex}\n"
$outfile[:plist].print "  </array>\n"
$outfile[:plist].print "</plist>\n"

$outfile.each do |key,file|
  file.close
end

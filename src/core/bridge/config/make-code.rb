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

  libxmldoc.root.find('//identifier').each do |identifier_node|
    if identifier_node['essential'] == 'true' then
      rawname = identifier_node.inner_xml
      name = rawname.gsub(/\./, '_')

      default_node = identifier_node.parent.find_first('./default')
      if default_node.nil? then
        $outfile[:cpp].print "0,\n"
      else
        $outfile[:cpp].print "#{default_node.inner_xml},\n"
      end

      $outfile[:hpp].print "BRIDGE_ESSENTIAL_CONFIG_INDEX_#{name} = #{configindex},\n"
      $outfile[:plist].print "    <string>#{rawname}</string>\n"
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

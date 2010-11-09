#!/usr/bin/ruby

require 'rubygems'
require 'xml/libxml'

GC.disable

$outfile = {
  :hpp => open('output/include.bridge_essential_config_index.hpp', 'w'),
  :cpp => open('output/include.bridge_essential_config_index.cpp', 'w'),
}

configindex = 0

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
      configindex += 1
    end
  end
end

$outfile.each do |key,file|
  file.close
end

#!/usr/bin/ruby

require 'rubygems'
require 'xml/libxml'

# ------------------------------------------------------------
file = "%s/../../files/prefpane/output/checkbox.xml" % File.dirname($0)

parser = XML::Parser.file(file)
libxmldoc = parser.parse

# remove <autogen> items.
libxmldoc.root.find('//sysctl').each do |node|
  node.parent.remove! unless node.parent.find_first('.//autogen').nil?
end

libxmldoc.root.find('//sysctl').each do |node|
  print node.children.to_s
  print "\n"
end

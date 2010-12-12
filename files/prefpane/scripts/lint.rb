#!/usr/bin/ruby
# -*- coding: utf-8 -*-

require 'rubygems'
require 'xml/libxml'

file = "%s/../output/checkbox.xml" % File.dirname($0)
parser = XML::Parser.file(file)
libxmldoc = parser.parse

libxmldoc.root.find('//identifier').each do |node|
  if /^private./ =~ node.inner_xml then
    print "\n"
    print "[ERROR] private.* identifier is forbidden in checkbox.xml.\n"
    p node
    print "\n"
    exit 1
  end
end

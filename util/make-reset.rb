#!/usr/bin/ruby
# -*- coding: undecided -*-

require 'rubygems'
require 'xml/libxml'

# ------------------------------------------------------------
ARGV.each do |filepath|
  parser = XML::Parser.file(filepath)
  libxmldoc = parser.parse

  libxmldoc.root.find('//sysctl').each do |node|
    entry = node.inner_xml

    # ----------------------------------------
    default = node.parent.find_first('./default')
    value = 0
    unless default.nil? then
      value = default.inner_xml
    end

    print "#{entry} #{value}\n"
  end
end

# static
print "debug 0\n"
print "debug_pointing 0\n"
print "debug_devel 0\n"

#!/usr/bin/ruby
# -*- coding: utf-8 -*-

require 'rubygems'
require 'xml/libxml'

system('make -C ../../files/prefpane > /dev/null')
file = "%s/../../files/prefpane/output/checkbox.xml" % File.dirname($0)
parser = XML::Parser.file(file)
libxmldoc = parser.parse


# <item> which has <list> must not have <identifier>.
#
#
# well-formed.
#
# <item>
#   <name>XXXX</name>
#   <list>
#   </list>
# <item>
#
#
# ill-formed.
#
# <item>
#   <name>XXXX</name>
#   <identifier>XXXX</identifier>
#   <list>
#   </list>
# <item>
#
libxmldoc.root.find('//list').each do |node|
  node.parent.children.each do |n|
    if n.name == 'identifier' then
      print "[ERROR]"
      p node.parent
      exit 1
    end
  end
end

# The contents of <identifier> must not begin in "private".
libxmldoc.root.find('//identifier').each do |node|
  if /^private/ =~ node.inner_xml then
    print "[ERROR] <identifier> begins in 'private'."
    p node.parent
    exit 1
  end
end

#!/usr/bin/ruby

require 'rubygems'
require 'xml/libxml'

system('make -C ../../files/prefpane > /dev/null')
file = "%s/../../files/prefpane/output/checkbox.xml" % File.dirname($0)
parser = XML::Parser.file(file)
libxmldoc = parser.parse

def traverse(node)
  node.children.each do |n|
    case n.name
    when 'list' then
      print "<ul>"
      traverse(n)
      print "</ul>\n"

    when 'item' then
      print "<li>"
      traverse(n)
      print "</li>"

    when 'name' then
      print n.inner_xml
      print "<br/>"

    when 'appendix' then
      print "&nbsp;&nbsp;#{n.inner_xml}<br/>"
    end
  end
end

traverse(libxmldoc.root)
print "\n\n"

total = libxmldoc.find('//identifier').count
print "total: #{total}\n"

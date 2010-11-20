#!/usr/bin/ruby

require 'rubygems'
require 'xml/libxml'

system('make -C ../../files/prefpane > /dev/null')
file = "%s/../../files/prefpane/output/checkbox.xml" % File.dirname($0)
parser = XML::Parser.file(file)
libxmldoc = parser.parse

libxmldoc.find('//list').each do |node_list|
  print "<ul>"
  node_list.find('./item').each do |node_item|
    print "<li>"
    node_item.find('./name').each do |node_name|
      print node_name.inner_xml
      print "<br/>"
    end
    node_item.find('./appendix').each do |node_appendix|
      print "  #{node_appendix.inner_xml}<br/>"
    end
    print "</li>"
  end
  print "</ul>\n"
end

print "\n\n"

total = libxmldoc.find('//identifier').count
print "total: #{total}\n"

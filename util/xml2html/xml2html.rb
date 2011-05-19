#!/usr/bin/ruby

require 'rubygems'
require 'xml/libxml'

system('make -C ../../files/prefpane > /dev/null')
file = "%s/../../files/prefpane/output/checkbox.xml" % File.dirname($0)
parser = XML::Parser.file(file)
libxmldoc = parser.parse

version = IO.read('../../version')

total = libxmldoc.find('//identifier').count
print "SUBJECT Version #{version.strip} (Total: #{total} prefs)\n"
print "RAW\n"

$first = true

def traverse(node)
  node.children.each do |n|
    case n.name
    when 'list' then
      if $first then
        print '<ul id="collapser">'
        $first = false
      else
        print '<ul>'
      end
      traverse(n)
      print "</ul>\n"

    when 'item' then
      print "<li>"
      traverse(n)
      print "</li>"

    when 'name' then
      print n.inner_xml.strip
      print "<br/>"

    when 'appendix' then
      print "&nbsp;&nbsp;#{n.inner_xml.strip}<br/>"
    end
  end
end

traverse(libxmldoc.root)
print "/RAW\n"

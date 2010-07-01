#!/usr/bin/ruby

require 'rubygems'
require 'xml/libxml'

system('make -C ../../files/prefpane > /dev/null')
file = "%s/../../files/prefpane/output/checkbox.xml" % File.dirname($0)
parser = XML::Parser.file(file)
libxmldoc = parser.parse


# <item> which has <list> must not have <sysctl>.
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
#   <sysctl>XXXX</sysctl>
#   <list>
#   </list>
# <item>
#
libxmldoc.root.find('//list').each do |node|
  node.parent.children.each do |n|
    if n.name == 'sysctl' then
      print "[ERROR]"
      p node.parent
      exit 1
    end
  end
end

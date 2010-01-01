#!/usr/bin/ruby

require 'rubygems'
require 'xml/libxml'

# ------------------------------------------------------------
enabled = {}

open("|sysctl -a | grep ^keyremap4macbook | grep -v ': 0$'") do |f|
  f.readlines.each do |l|
    if /(.+?):/ =~ l then
      enabled[$1] = true
    end
  end
end

# ------------------------------------------------------------
file = "%s/../files/prefpane/output/checkbox.xml" % File.dirname($0)

parser = XML::Parser.file(file)
libxmldoc = parser.parse

# remove disabled items.
libxmldoc.root.find('//sysctl').each do |node|
  name = "keyremap4macbook.#{node.children.to_s}"
  node.parent.remove! unless enabled[name] == true
end

# cleanup
libxmldoc.root.find('//autogen').each do |node|
  node.remove!
end

while true
  hit = false
  libxmldoc.root.find('//item').each do |node|
    if node.find_first('.//sysctl').nil?
      node.remove!
      hit = true
      break
    end
  end
  break unless hit
end

print libxmldoc

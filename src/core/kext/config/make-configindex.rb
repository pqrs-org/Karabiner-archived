#!/usr/bin/ruby
# -*- coding: utf-8 -*-

$outfile = {
  :configindex => open('../keycode/data/include/ConfigIndex.GENERATED.data', 'w'),
}

# ======================================================================
require 'rubygems'
require 'xml/libxml'
require 'inc.preprocess.rb'

index = 0
parser = XML::Parser.file('../../../../files/prefpane/output/checkbox.xml')
libxmldoc = parser.parse

# ----------------------------------------
libxmldoc.root.find('//sysctl').each do |node|
  next if node['essential'] == "true"
  name = node.children.map{|n| n.to_s}.join('')
  name.gsub!(/\./, '_')

  $outfile[:configindex] << "#{name} #{index}\n"
  index += 1
end

# ======================================================================
$outfile.each do |name,file|
  file.close
end

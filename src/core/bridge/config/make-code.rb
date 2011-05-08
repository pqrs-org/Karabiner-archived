#!/usr/bin/ruby

require 'rubygems'
require 'xml/libxml'

GC.disable

# output to tmpfile
$outfile = {
  :hpp   => open('output/include.bridge_essential_config_index.hpp.tmp', 'w'),
  :cpp   => open('output/include.bridge_essential_config_index.cpp.tmp', 'w'),
  :plist => open('output/include.bridge_essential_config_index.plist.tmp', 'w'),
}

configindex = 0

$outfile[:plist].print '<?xml version="1.0" encoding="UTF-8"?>' + "\n"
$outfile[:plist].print '<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">' + "\n"
$outfile[:plist].print '<plist version="1.0">' + "\n"
$outfile[:plist].print '  <array>' + "\n"

ARGV.each do |xmlpath|
  parser = XML::Parser.file(xmlpath)
  libxmldoc = parser.parse

  libxmldoc.root.find('//identifier').each do |identifier_node|
    if identifier_node['essential'] == 'true' then
      rawname = identifier_node.inner_xml
      name = rawname.gsub(/\./, '_')

      if identifier_node['default'].nil? then
        $outfile[:cpp].print "0,\n"
      else
        $outfile[:cpp].print "#{identifier_node['default']},\n"
      end

      $outfile[:hpp].print "BRIDGE_ESSENTIAL_CONFIG_INDEX_#{name} = #{configindex},\n"
      $outfile[:plist].print "    <string>#{rawname}</string>\n"
      configindex += 1
    end
  end
end

$outfile[:hpp].print "BRIDGE_ESSENTIAL_CONFIG_INDEX__END__ = #{configindex}\n"
$outfile[:plist].print "  </array>\n"
$outfile[:plist].print "</plist>\n"

$outfile.each do |key,file|
  file.close
end

# ------------------------------------------------------------
# move tmpfile if needed.
$outfile.each do |key,file|
  tmpfilepath    = file.path
  targetfilepath = file.path.gsub(/\.tmp$/, '')

  if (! FileTest.exist?(targetfilepath)) or (IO.read(tmpfilepath) != IO.read(targetfilepath)) then
    File.rename(tmpfilepath, targetfilepath)
  else
    File.unlink(tmpfilepath)
  end
end

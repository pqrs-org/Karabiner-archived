#!/usr/bin/ruby

require "#{File.dirname(__FILE__)}/../lib/converter.rb"

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
  xml = File.read(xmlpath)

  xml.scan(/<identifier(\s.*?)?>(.+?)<\/identifier>/m).each do |matches|
    identifier = matches[1].strip
    attr = matches[0]
    if /\sessential=['"]true['"]/ =~ attr then
      name = identifier.gsub(/\./, '_')

      unless /\sdefault=['"](.+?)['"]/ =~ attr then
        $outfile[:cpp].print "0,\n"
      else
        $outfile[:cpp].print "#{$1},\n"
      end

      $outfile[:hpp].print "BRIDGE_ESSENTIAL_CONFIG_INDEX_#{name} = #{configindex},\n"
      $outfile[:plist].print "    <string>#{identifier}</string>\n"
      configindex += 1
    end
  end
end

$outfile[:hpp].print "BRIDGE_ESSENTIAL_CONFIG_INDEX__END__ = #{configindex}\n"
$outfile[:plist].print "  </array>\n"
$outfile[:plist].print "</plist>\n"

$outfile.each do |key,file|
  file.close
  KeyRemap4MacBookBridge::Converter.update_file_if_needed(file.path)
end

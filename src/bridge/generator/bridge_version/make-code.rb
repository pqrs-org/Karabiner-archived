#!/usr/bin/ruby

require "#{File.dirname(__FILE__)}/../lib/converter.rb"

filepath = "../../output/include.bridge_version.h.tmp"
open(filepath, 'w') do |f|
  open('../../../../version', 'r') do |version|
    a = version.read.split(/\./)
    f << ("0x%02df%02df%02d" % [a[0], a[1], a[2]])
    f << "\n"
  end
end

KeyRemap4MacBookBridge::Converter.update_file_if_needed(filepath)

#!/usr/bin/ruby
# -*- coding: utf-8 -*-

file = "%s/../output/checkbox.xml" % File.dirname($0)
xml = File.read(file)
xml.scan(/<identifier(\s.*?)?>(.+?)<\/identifier>/m).each do |matches|
  identifier = matches[1].strip
  if /^private./ =~ identifier or /^notsave.private/ =~ identifier then
    print "\n"
    print "[ERROR] private.* identifier is forbidden in checkbox.xml.\n"
    p identifier
    print "\n"
    exit 1
  end
end

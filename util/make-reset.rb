#!/usr/bin/env ruby

entry = nil
value = 0
while l = gets
  if /<sysctl>(.+)<\/sysctl>/ =~ l then
    newentry = $1

    unless entry.nil? then
      print "#{entry} #{value}\n"
    end
    entry = newentry
    value = 0
  end
  if /<default>(.+)<\/default>/ =~ l then
    value = $1
  end
end

unless entry.nil? then
  print "#{entry} #{value}\n"
end

# static
print "notsave.pointing_relative_to_scroll 0\n"
print "debug 0\n"
print "debug_pointing 0\n"
print "debug_devel 0\n"

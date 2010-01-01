#!/usr/bin/ruby

require 'cgi'

lines = []
if ARGV[0] == '-' then
  lines = $stdin.readlines
else
  system('make -C ../../files/prefpane > /dev/null')
  file = "%s/../../files/prefpane/output/checkbox.xml" % File.dirname($0)
  open(file) do |f|
    lines = f.readlines
  end
end

total = 0
lines.each do |l|
  if /<list>/ =~ l then
    print "<ul>"
  end
  if /<\/list>/ =~ l then
    print "</ul>"
  end
  if /<item>/ =~ l then
    print "<li>"
  end
  if /<\/item>/ =~ l then
    print "</li>"
  end
  if /<name>(.+)<\/name>/ =~ l then
    print $1
  end
  if /<appendix>(.+)<\/appendix>/ =~ l then
    print "<br />", $1
  end
  if /<sysctl>/ =~ l then
    total += 1
  end
end
print "\n\n"
print "total: #{total}\n"

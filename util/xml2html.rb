#!/usr/bin/ruby

require 'cgi'


file = "%s/../files/prefpane/checkbox.xml" % File.dirname($0)
open(file) do |f|
  while l = f.gets
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
      print CGI.escapeHTML($1)
    end
  end
  print "\n"
end

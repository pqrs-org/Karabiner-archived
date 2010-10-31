#!/usr/bin/ruby

Dir.chdir("data")

alldata = []

Dir.glob("*.data") do |filename|
  if /(.+)\.data/ =~ File.basename(filename) then
    classname = $1
    lastvalue = 0

    outfile = {
      :hpp => open("../output/include.#{classname}.hpp", "w"),
    }

    open(filename) do |f|
      while l = f.gets
        if /^\/\// =~ l then
          outfile[:hpp] << l
        elsif /^(.+?)\s+(.+)$/ =~ l then
          name = $1
          value = $2

          if value == '--AUTO--' then
            value = lastvalue.succ
          end
          lastvalue = value

          outfile[:hpp] << "static const #{classname} #{name};\n"
          alldata << { :name => "#{classname}::#{name}", :value => value, :classname => classname }
        else
          unless l.strip.empty? then
            outfile[:hpp] << "ERROR #{l}\n"
          end
        end
      end
    end

    outfile[:hpp] << "static const #{classname} VK__AUTOINDEX__BEGIN__;\n"
    alldata << { :name => "#{classname}::VK__AUTOINDEX__BEGIN__", :value => lastvalue.succ, :classname => classname }

    outfile.each do |name,file|
      file.close
    end
  end
end

# ----------------------------------------------------------------------
# output plist
# http://developer.apple.com/library/mac/#documentation/Cocoa/Conceptual/PropertyLists/QuickStartPlist/QuickStartPlist.html
plist = []
plist << '<?xml version="1.0" encoding="UTF-8"?>'
plist << '<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">'
plist << '<plist version="1.0">'
plist << '  <dict>'
alldata.each do |info|
  plist << "    <key>#{info[:name]}</key>"
  plist << "    <integer>#{info[:value]}</integer>"
end
plist << '  </dict>'
plist << '</plist>'

open("../output/include.keycode.plist", 'w') do |f|
  f << plist.join("\n")
end

# ----------------------------------------------------------------------
# output cpp
open("../output/include.keycode.cpp", 'w') do |f|
  alldata.each do |info|
    f << "const #{info[:classname]} #{info[:name]}(#{info[:value]});\n"
  end
end

# ----------------------------------------------------------------------
# output raw
open("../output/include.keycode.raw", 'w') do |f|
  alldata.each do |info|
    f << "#{info[:name]} #{info[:value]}\n"
  end
end

#!/usr/bin/ruby

Dir.chdir("data")

alldata = []

Dir.glob("*.data") do |filename|
  if /(.+)\.data/ =~ File.basename(filename) then
    classname = $1
    lastvalue = 0

    outfile = {
      :hpp => open("../output/include.#{classname}.hpp", "w"),
      :cpp => open("../output/include.#{classname}.cpp", "w"),
      :raw => open("../output/include.#{classname}.raw", "w"),
    }

    open(filename) do |f|
      while l = f.gets
        if /^\/\// =~ l then
          outfile[:hpp] << l
          outfile[:cpp] << l
        elsif /^(.+?)\s+(.+)$/ =~ l then
          name = $1
          value = $2

          if value == '--AUTO--' then
            value = lastvalue.succ
          end
          lastvalue = value

          outfile[:hpp] << "static const #{classname} #{name};\n"
          outfile[:cpp] << "const #{classname} #{classname}::#{name}(#{value});\n"
          outfile[:raw] << "#{classname}::#{name} #{value}\n"
          alldata << { :name => "#{classname}::#{name}", :value => value }
        else
          unless l.strip.empty? then
            outfile[:hpp] << "ERROR #{l}\n"
            outfile[:cpp] << "ERROR #{l}\n"
          end
        end
      end
    end

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

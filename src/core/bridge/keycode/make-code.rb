#!/usr/bin/ruby

require "#{File.dirname(__FILE__)}/../lib/converter.rb"

Dir.chdir("data")

alldata = []

Dir.glob("*.data") do |filename|
  if /(.+)\.data/ =~ File.basename(filename) then
    classname = $1
    lastvalue = 0

    outfile = {
      :hpp => open("../output/include.#{classname}.hpp.tmp", "w"),
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
          alldata << {
            :name      => "#{classname}::#{name}",
            :value     => value,
            :classname => classname,
            :nameonly  => name,
          }
        else
          unless l.strip.empty? then
            outfile[:hpp] << "ERROR #{l}\n"
          end
        end
      end
    end

    outfile[:hpp] << "static const #{classname} VK__AUTOINDEX__BEGIN__;\n"
    alldata << {
      :name      => "#{classname}::VK__AUTOINDEX__BEGIN__",
      :value     => lastvalue.succ,
      :classname => classname,
      :nameonly  => "VK__AUTOINDEX__BEGIN__",
    }

    outfile.each do |name,file|
      file.close
      KeyRemap4MacBookBridge::Converter.update_file_if_needed(file.path)
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

filepath = "../output/include.keycode.plist.tmp"
open(filepath, 'w') do |f|
  f << plist.join("\n")
end
KeyRemap4MacBookBridge::Converter.update_file_if_needed(filepath)

# ----------------------------------------------------------------------
# output cpp
filepath = "../output/include.keycode.cpp.tmp"
open(filepath, 'w') do |f|
  alldata.each do |info|
    f << "const #{info[:classname]} #{info[:name]}(#{info[:value]});\n"
  end
end
KeyRemap4MacBookBridge::Converter.update_file_if_needed(filepath)

# ----------------------------------------------------------------------
# output raw
filepath = "../output/include.keycode.raw.tmp"
open(filepath, 'w') do |f|
  alldata.each do |info|
    f << "#{info[:name]} #{info[:value]}\n"
  end
end
KeyRemap4MacBookBridge::Converter.update_file_if_needed(filepath)

# ----------------------------------------------------------------------
# output xml
filepath = "../output/symbolmap.xml.tmp"
open(filepath, 'w') do |f|
  f << "<?xml version=\"1.0\"?>\n"
  f << "<root>\n"
  alldata.each do |info|
    f << "  <item type=\"#{info[:classname]}\" name=\"#{info[:nameonly]}\" value=\"#{info[:value]}\" />\n"
  end
  f << "</root>\n"
end

KeyRemap4MacBookBridge::Converter.update_file_if_needed(filepath)

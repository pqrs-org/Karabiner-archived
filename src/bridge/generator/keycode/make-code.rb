#!/usr/bin/ruby

require "#{File.dirname(__FILE__)}/../lib/converter.rb"

Dir.chdir("data")

alldata = []

Dir.glob("*.data") do |filename|
  if /(.+)\.data/ =~ File.basename(filename) then
    classname = $1
    lastvalue = 0

    outfile = {
      :hpp => open("../../../output/include.kext.#{classname}.hpp.tmp", "w"),
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
# output cpp
filepath = "../../../output/include.kext.keycode.cpp.tmp"
open(filepath, 'w') do |f|
  alldata.each do |info|
    f << "const #{info[:classname]} #{info[:name]}(#{info[:value]});\n"
  end
end
KeyRemap4MacBookBridge::Converter.update_file_if_needed(filepath)

# ----------------------------------------------------------------------
# symbol_map.xml
filepath = "../../../../core/server/Resources/symbol_map.xml.tmp"
open(filepath, 'w') do |f|
  f << "<?xml version=\"1.0\"?>\n"
  f << "<root>\n"
  alldata.each do |info|
    f << "  <symbol_map type=\"#{info[:classname]}\" name=\"#{info[:nameonly]}\" value=\"#{info[:value]}\" />\n"
  end
  f << "</root>\n"
end

KeyRemap4MacBookBridge::Converter.update_file_if_needed(filepath)

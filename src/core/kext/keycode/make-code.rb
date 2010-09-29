#!/usr/bin/ruby

Dir.chdir("data")

Dir.glob("*.data") do |filename|
  if /(.+)\.data/ =~ File.basename(filename) then
    classname = $1
    lastvalue = 0

    outfile = {
      :hpp => open("../output/include.#{classname}.hpp", "w"),
      :cpp => open("../output/include.#{classname}.cpp", "w"),
      :raw => open("../output/include.#{classname}.raw", "w"),
    }

    open("| cpp -P #{filename}") do |f|
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

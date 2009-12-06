#!/usr/bin/ruby

Dir.glob("data/*.data") do |filename|
  if /(.+)\.data/ =~ File.basename(filename) then
    classname = $1
    lastvalue = 0
    open("output/include.#{classname}.hpp", "w") do |of_hpp|
      open("output/include.#{classname}.cpp", "w") do |of_cpp|
        open(filename) do |f|
          while l = f.gets
            if /^\/\// =~ l then
              of_hpp.print l
              of_cpp.print l
            elsif /^(.+?)\s+(.+)$/ =~ l then
              name = $1
              value = $2

              if value == '--AUTO--' then
                value = lastvalue.succ
              end
              lastvalue = value

              of_hpp.print "static const #{classname} #{name};\n"
              of_cpp.print "const #{classname} #{classname}::#{name}(#{value});\n"
            else
              unless l.strip.empty? then
                of_hpp.print "ERROR #{l}\n"
                of_cpp.print "ERROR #{l}\n"
              end
            end
          end
        end
      end
    end
  end
end


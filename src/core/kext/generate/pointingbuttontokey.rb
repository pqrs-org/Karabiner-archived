#!/usr/bin/env ruby
# -*- coding: utf-8 -*-

# generate PointingButtonToKey combination.

num = ARGV[0].to_i
max = ARGV[1].to_i

if max == 0 then
  $stderr.print "Usage: pointingbuttontokey.rb num max\n"
  exit
end

def makeset(n)
  if n == 0 then
    []
  elsif n == 1 then
    [[true], [false]]
  else
    a = makeset(n - 1)
    set = []
    a.each {|i| set << i + [true] }
    a.each {|i| set << i + [false] }
    set
  end
end

makeset(num+1).each do |omitflags|
  isalltrue = omitflags.all?

  print "bool remap(RemapPointingParams_relative& remapParams,\n"
  code = "return remap(remapParams,\n               fromButton"

  if omitflags.shift then
    print "           PointingButton fromButton, Flags fromFlags,\n"
    code += ", fromFlags"
  else
    print "           PointingButton fromButton,\n"
    code += ", Flags(0)"
  end

  i = 1
  omitflags.each do |flag|
    text = nil
    if flag then
      text = "           KeyCode toKeyCode#{i},  Flags toFlags#{i}"
      code += ",\n               toKeyCode#{i}, toFlags#{i}"
    else
      text = "           KeyCode toKeyCode#{i}"
      code += ",\n               toKeyCode#{i}, ModifierFlag::NONE"
    end
    if i < num then
      print "#{text},\n"
    else
      if (num < max) || ! isalltrue then
        (max - num).times do
          code += ",\n               KeyCode::VK_NONE, ModifierFlag::NONE"
        end
        print "#{text}) {\n"
        print "  #{code});\n"
        print "}\n\n"
      else
        print "#{text});\n\n"
      end
    end
    i += 1
  end
end

#!/usr/bin/env ruby
# -*- coding: utf-8 -*-

# generate KeyToKey combination.

num = ARGV[0].to_i
max = ARGV[1].to_i

if max == 0 then
  $stderr.print "Usage: keytokey.rb num max\n"
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

  print "bool remap(RemapParams& remapParams,\n"
  code = "return remap(remapParams, fromKeyCode"

  if omitflags.shift then
    print "           const KeyCode& fromKeyCode, const Flags& fromFlags,\n"
    code += ", fromFlags"
  else
    print "           const KeyCode& fromKeyCode,\n"
    code += ", Flags(0)"
  end

  i = 1
  omitflags.each do |flag|
    text = nil
    if flag then
      text = "           const KeyCode& toKeyCode#{i},  const Flags& toFlags#{i}"
      code += ", toKeyCode#{i}, toFlags#{i}"
    else
      text = "           const KeyCode& toKeyCode#{i}"
      code += ", toKeyCode#{i}, ModifierFlag::NONE"
    end
    if i < num then
      print "#{text},\n"
    else
      if (num < max) || ! isalltrue then
        (max - num).times do
          code += ", KeyCode::VK_NONE, ModifierFlag::NONE"
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


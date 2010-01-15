#!/usr/bin/env ruby
# -*- coding: utf-8 -*-

# generate KeyOverlaidModifier combination.

num = ARGV[0].to_i
max = ARGV[1].to_i

if max == 0 then
  $stderr.print "Usage: keyoverlaidmodifier.rb num max\n"
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

  params = ''
  code = "return remap(remapParams,\n             fromKeyCode"

  if omitflags.shift then
    params += "           KeyCode fromKeyCode,   Flags fromFlags,\n"
    code += ", fromFlags"
  else
    params += "           KeyCode fromKeyCode,\n"
    code += ", Flags(0)"
  end

  i = 1
  omitflags.each do |flag|
    if i == 1 then
      if flag then
        params += "           KeyCode toKeyCode,     Flags toFlags"
        code += ",\n             toKeyCode, toFlags"
      else
        params += "           KeyCode toKeyCode"
        code += ",\n             toKeyCode, ModifierFlag::NONE"
      end
    else
      if flag then
        params += "           KeyCode fireKeyCode#{i-1},  Flags fireFlags#{i-1}"
        code += ",\n             fireKeyCode#{i-1}, fireFlags#{i-1}"
      else
        params += "           KeyCode fireKeyCode#{i-1}"
        code += ",\n             fireKeyCode#{i-1}, ModifierFlag::NONE"
      end
    end

    if i < num then
      params += ",\n"
    end
    i += 1
  end

  if (num < max) || ! isalltrue then
    (max - num).times do
      code += ",\n             KeyCode::VK_NONE, ModifierFlag::NONE"
    end
  end

  # ------------------------------------------------------------
  if (num < max) || ! isalltrue then
    print "bool remap(RemapParams& remapParams,\n"
    print "#{params},\n"
    print "           bool isFireRepeat = false) {\n"
    print "#{code},\n"
    print "             isFireRepeat);\n"
    print "}\n\n"

    print "bool remapWithRepeat(RemapParams& remapParams,\n"
    print "#{params}) {\n"
    print "#{code},\n"
    print "             true);\n"
    print "}\n\n"

  else
    print "bool remap(RemapParams& remapParams,\n"
    print "#{params},\n"
    print "           bool isFireRepeat = false);\n\n"

    print "bool remapWithRepeat(RemapParams& remapParams,\n"
    print "#{params}) {\n"
    print "#{code},\n"
    print "             true);\n"
    print "}\n\n"
  end
end

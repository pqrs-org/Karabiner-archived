#!/usr/bin/env ruby
# -*- coding: utf-8 -*-

# generate KeyEventInputQueue::Remap combination.

num = ARGV[0].to_i
max = ARGV[1].to_i

if max == 0 then
  $stderr.print "Usage: keyeventinputqueue.rb num max\n"
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

makeset(num).each do |omitflags|
  print "void initialize(KeyCode virtualKeyCode, KeyCode fromKeyCode1, KeyCode fromKeyCode2"
  code = ''
  code += "  virtualKeyCode_ = virtualKeyCode;\n\n"
  code += "  fromKeyCode1_ = fromKeyCode1;\n"
  code += "  fromKeyCode2_ = fromKeyCode2;\n\n"

  i = 1
  omitflags.each do |flag|
    if flag then
      print ",\n      KeyCode toKeyCode#{i}, Flags toFlags#{i}"
      code += "  toKeyCode#{i}_ = toKeyCode#{i};\n"
      code += "  toFlags#{i}_ = toFlags#{i};\n\n"
    else
      print ",\n      KeyCode toKeyCode#{i}"
      code += "  toKeyCode#{i}_ = toKeyCode#{i};\n"
      code += "  toFlags#{i}_ = ModifierFlag::NONE;\n\n"
    end
    i += 1
  end
  (num+1 .. max).each do |i|
    code += "  toKeyCode#{i}_ = KeyCode::VK_NONE;\n"
    code += "  toFlags#{i}_ = ModifierFlag::NONE;\n\n"
  end

  print ")\n{\n"
  print code
  print "  active1_ = false;\n"
  print "  active2_ = false;\n"
  print "}\n"
end

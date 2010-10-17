#!/usr/bin/ruby
# -*- coding: utf-8 -*-

class KeyCode
  @@keycode = {}

  def KeyCode.load_keycode
    return unless @@keycode.empty?

    Dir.glob("../keycode/output/*.raw").each do |filepath|
      type = File.basename(filepath, 'raw').gsub(/^include\./, '')
      open(filepath) do |f|
        while l = f.gets
          next unless /^(.+?)::(.+?) (.+)$/ =~ l
          if @@keycode[$1].nil? then
            @@keycode[$1] = {}
          end
          @@keycode[$1][$2] = $3
        end
      end
    end
  end

  def KeyCode.v(type, key)
    load_keycode
    value = @@keycode[type][key]
    if value.nil?
      $stderr.print "unknown KeyCode[#{type}][#{key}]\n"
      throw :exit
    end
    value
  end

  def KeyCode.[](key)
    a = key.split(/::/)
    KeyCode.v(a[0], a[1])
  end

  def KeyCode.ConfigIndex(key)
    KeyCode.v('ConfigIndex', key)
  end

  def KeyCode.count(type)
    @@keycode[type].count
  end
end

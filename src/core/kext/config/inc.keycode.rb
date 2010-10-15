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
          next unless /^(.+?) (.+)$/ =~ l
          @@keycode[$1] = $2
        end
      end
    end
  end

  def KeyCode.[](key)
    load_keycode
    @@keycode[key]
  end
end

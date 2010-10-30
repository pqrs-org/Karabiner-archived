#!/usr/bin/ruby
# -*- coding: utf-8 -*-

class KeyCode
  @@keycode = {}
  @@autoindex = {}
  @@configindex = 0

  def KeyCode.load_keycode
    return unless @@keycode.empty?

    Dir.glob("../../bridge/keycode/output/*.raw").each do |filepath|
      type = File.basename(filepath, 'raw').gsub(/^include\./, '')
      open(filepath) do |f|
        while l = f.gets
          next unless /^(.+?)::(.+?) (.+)$/ =~ l
          if @@keycode[$1].nil? then
            @@keycode[$1] = {}
          end
          @@keycode[$1][$2] = $3
          @@autoindex[$1] = $3.succ
        end
      end
    end
  end

  def KeyCode.setup_configindex(node)
    if @@keycode['ConfigIndex'].nil? then
      @@keycode['ConfigIndex'] = {}
    end

    node.find('//sysctl').each do |node|
      next if node['essential'] == "true"
      name = node.inner_xml.gsub(/\./, '_')

      @@keycode['ConfigIndex'][name] = @@configindex
      @@configindex += 1
    end
  end

  def KeyCode.append_autoindexed(type, key)
    @@keycode[type][key] = @@autoindex[type].dup
    @@autoindex[type].succ!
  end

  def KeyCode.v(type, key)
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

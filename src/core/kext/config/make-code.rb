#!/usr/bin/ruby

# ----------------------------------------------------------------------
def fileread(filename)
  text = nil
  open(filename) do |f|
    text = f.read
  end
  text
end

template_KeyOverlaidModifierCombination = fileread('template/KeyOverlaidModifierCombination.cpp')

# ----------------------------------------------------------------------
lastName = nil;
while l = $stdin.gets
  l.strip!

  if /<sysctl>(.+?)\.(.+)<\/sysctl>/ =~ l then
    type = $1
    entry = $2

    lastName = "#{type}_#{entry}"

    case ARGV[0]
    when 'hpp'
      print "int #{type}_#{entry};\n"
    when 'cpp_SYSCTL'
      print "SYSCTL_INT(_keyremap4macbook_#{type}, OID_AUTO, #{entry}, CTLTYPE_INT|CTLFLAG_RW, &(config.#{type}_#{entry}), 0, \"\");\n"
    when 'cpp_register'
      print "sysctl_register_oid(&sysctl__keyremap4macbook_#{type}_#{entry});\n"
    when 'cpp_unregister'
      print "sysctl_unregister_oid(&sysctl__keyremap4macbook_#{type}_#{entry});\n"
    end
  end

  if /<default>(.+)<\/default>/ =~ l then
    value = $1
    if ARGV[0] == 'default' then
      print "#{lastName} = #{value};\n"
    end
  end

  if /<autogen>(.+)<\/autogen>/ =~ l then
    value = $1
    if /^(.+?):(.+)$/ =~ value then
      type = $1
      params = $2

      case ARGV[0]
      when 'remapcode_func'
        case type
        when 'KeyOverlaidModifierCombination'
          print template_KeyOverlaidModifierCombination.gsub(/%%LASTNAME%%/, lastName).gsub(/%%PARAMS%%/, params)
        end

      when 'remapcode_call_komc'
        if type == 'KeyOverlaidModifierCombination' then
          print "#{lastName}(params);\n"
        end
      end
    end
  end
end

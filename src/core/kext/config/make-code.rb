#!/usr/bin/ruby

# ----------------------------------------------------------------------
template = {}
Dir.glob('template/*.cpp') do |filename|
  key = File.basename(filename, '.cpp')
  open(filename) do |f|
    template[key] = f.read
  end
end

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
    if /^--(.+?)-- (.+)$/ =~ value then
      type = $1
      params = $2

      case ARGV[0]
      when 'remapcode_func'
        code = template[type]
        if code.nil? then
          $stdout.puts "[ERROR] There is no template for #{type}"
        else
          print code.gsub(/%%LASTNAME%%/, lastName).gsub(/%%PARAMS%%/, params)
        end

      when 'remapcode_call'
        if type == 'KeyToKey' || type == 'KeyToPointingButton' then
          print "GeneratedCode::#{lastName}(params);\n"
        end

      when 'remapcode_call_kom'
        if type == 'KeyOverlaidModifier' then
          print "GeneratedCode::#{lastName}(params);\n"
        end

      when 'remapcode_call_komc'
        if type == 'KeyOverlaidModifierCombination' then
          print "GeneratedCode::#{lastName}(params);\n"
        end

      when 'remapcode_call_consumer'
        if type == 'ConsumerToKey' then
          print "GeneratedCode::#{lastName}(params);\n"
        end
      end
    end
  end
end

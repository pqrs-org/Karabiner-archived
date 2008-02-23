#!/usr/bin/ruby

def output(filename, type)
  open(filename) do |f|
    while l = f.gets
      next if /^\s*#/ =~ l

      l.strip!

      case ARGV[0]
      when 'hpp'
        print "int #{type}_#{l};\n"
      when 'cpp_SYSCTL'
        print "SYSCTL_INT(_keyremap4macbook_#{type}, OID_AUTO, #{l}, CTLTYPE_INT|CTLFLAG_RW, &(config.#{type}_#{l}), 0, \"\");\n"
      when 'cpp_register'
        print "sysctl_register_oid(&sysctl__keyremap4macbook_#{type}_#{l});\n"
      when 'cpp_unregister'
        print "sysctl_unregister_oid(&sysctl__keyremap4macbook_#{type}_#{l});\n"
      end
    end
  end
end

output('config.remap', 'remap');
output('config.option', 'option');

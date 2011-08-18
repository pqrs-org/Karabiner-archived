#!/usr/bin/ruby

$lines = []
while l = $stdin.gets
  $lines << l.strip
end

def parseline(line)
  if /(.+?)=(.+)/ =~ line then
    name = $1
    value = $2
    return [name.strip, value.strip.gsub(/;$/, '')]
  end
  return [nil, nil]
end

def check_value(name, value)
  return if ARGV.include?(name)
  print "  check #{name}\n"

  isexist = false
  $lines.each do |l|
    (linename, linevalue) = parseline(l)
    if name == linename then
      isexist = true
      if value != linevalue then
        print "[ERROR] Invalid value: #{name} (#{linevalue} != #{value})\n"
        exit 1
      end
    end
  end
  unless isexist then
    print "[ERROR] No setting: #{name}\n"
    exit 1
  end
end

def check_noexist(name)
  $lines.each do |l|
    (linename, linevalue) = parseline(l)
    if name == linename then
      print "[ERROR] Appear name: #{name}\n"
      exit 1
    end
  end
end

check_value('ARCHS', '"$(ARCHS_STANDARD_32_64_BIT)"');
check_value('objectVersion', '46')
check_value('GCC_ENABLE_OBJC_GC', 'unsupported')
check_value('GCC_TREAT_WARNINGS_AS_ERRORS', 'YES')
check_value('GCC_WARN_64_TO_32_BIT_CONVERSION', 'YES')
check_value('GCC_WARN_ABOUT_MISSING_NEWLINE', 'YES')
check_value('GCC_WARN_ABOUT_MISSING_PROTOTYPES', 'YES')
check_value('GCC_WARN_ABOUT_RETURN_TYPE', 'YES')
check_value('GCC_WARN_CHECK_SWITCH_STATEMENTS', 'YES')
check_value('GCC_WARN_MISSING_PARENTHESES', 'YES')
check_value('GCC_WARN_SHADOW', 'YES')
check_value('GCC_WARN_SIGN_COMPARE', 'YES')
check_value('GCC_WARN_UNDECLARED_SELECTOR', 'YES')
check_value('GCC_WARN_UNINITIALIZED_AUTOS', 'YES')
check_value('GCC_WARN_UNUSED_FUNCTION', 'YES')
check_value('GCC_WARN_UNUSED_LABEL', 'YES')
check_value('GCC_WARN_UNUSED_VALUE', 'YES')
check_value('GCC_WARN_UNUSED_VARIABLE', 'YES')
check_value('RUN_CLANG_STATIC_ANALYZER', 'YES')
check_value('SDKROOT', 'macosx10.6')

check_noexist('GCC_WARN_PROTOTYPE_CONVERSION')

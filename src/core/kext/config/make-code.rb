#!/usr/bin/ruby

require 'inc.filter.rb'

$outfile = {
  :config => open('output/include.config.hpp', 'w'),
  :config_SYSCTL => open('output/include.config_SYSCTL.cpp', 'w'),
  :config_register => open('output/include.config_register.cpp', 'w'),
  :config_unregister => open('output/include.config_unregister.cpp', 'w'),
  :config_default => open('output/include.config.default.hpp', 'w'),
  :remapcode_keyboardtype => open('output/include.remapcode_keyboardtype.hpp', 'w'),
  :remapcode_func => open('output/include.remapcode_func.cpp', 'w'),
  :remapcode_info => open('output/include.remapcode_info.cpp', 'w'),
  :remapcode_refresh_remapfunc_key => open('output/include.remapcode_refresh_remapfunc_key.cpp', 'w'),
  :remapcode_refresh_remapfunc_consumer => open('output/include.remapcode_refresh_remapfunc_consumer.cpp', 'w'),
  :remapcode_refresh_remapfunc_pointing => open('output/include.remapcode_refresh_remapfunc_pointing.cpp', 'w'),
  :remapcode_refresh_remapfunc_statusmessage => open('output/include.remapcode_refresh_remapfunc_statusmessage.cpp', 'w'),
  :remapcode_vk_config => open('output/include.remapcode_vk_config.cpp', 'w'),
  :remapcode_simultaneouskeypresses_func => open('output/include.remapcode_simultaneouskeypresses_func.cpp', 'w'),
  :keycode_vk_config => open('../keycode/data/include/KeyCode.VK_GENERATED.data', 'w'),
}

$func = {
  :key => [],
  :consumer => [],
  :pointing => [],

  :simultaneouskeypresses => [],
}

# ======================================================================
def getextrakey(key)
  case key
  when 'HOME'
    'CURSOR_LEFT'
  when 'END'
    'CURSOR_RIGHT'
  when 'PAGEUP'
    'CURSOR_UP'
  when 'PAGEDOWN'
    'CURSOR_DOWN'
  when 'FORWARD_DELETE'
    'DELETE'
  else
    nil
  end
end

def preprocess(listAutogen)
  modify = false

  list = []
  listAutogen.each do |autogen|
    if /VK_(COMMAND|CONTROL|SHIFT|OPTION)/ =~ autogen then
      key = $1
      list << autogen.gsub(/VK_#{key}/, "ModifierFlag::#{key}_L")
      list << autogen.gsub(/VK_#{key}/, "ModifierFlag::#{key}_R")
      modify = true
    elsif /VK_MOD_CCOS_L/ =~ autogen then
      list << autogen.gsub(/VK_MOD_CCOS_L/, "ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::OPTION_L | ModifierFlag::SHIFT_L")
      modify = true
    elsif /VK_MOD_CCS_L/ =~ autogen then
      list << autogen.gsub(/VK_MOD_CCS_L/, "ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L")
      modify = true
    elsif /VK_MOD_CCO_L/ =~ autogen then
      list << autogen.gsub(/VK_MOD_CCO_L/, "ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::OPTION_L")
      modify = true
    elsif /VK_MOD_ANY/ =~ autogen then
      # to reduce combination, we ignore same modifier combination such as (COMMAND_L | COMMAND_R).
      modifiers = [
                   'VK_COMMAND',
                   'VK_CONTROL',
                   'ModifierFlag::FN',
                   'VK_OPTION',
                   'VK_SHIFT',
                  ]
      (0 .. modifiers.length).to_a.reverse.each do |i|
        modifiers.combination(i).to_a.each do |pattern|
          pat = pattern.dup
          pat << "ModifierFlag::NONE"
          list << autogen.gsub(/VK_MOD_ANY/, pat.join(' | '))
        end
      end
      modify = true

    elsif /FROMKEYCODE_(HOME|END|PAGEUP|PAGEDOWN|FORWARD_DELETE)\s*,\s*ModifierFlag::/ =~ autogen then
      key = $1
      extrakey = getextrakey(key)
      list << autogen.gsub(/FROMKEYCODE_#{key}\s*,/, "KeyCode::#{key},")
      list << autogen.gsub(/FROMKEYCODE_#{key}\s*,/, "KeyCode::#{extrakey}, ModifierFlag::FN |")
      modify = true
    elsif /FROMKEYCODE_(HOME|END|PAGEUP|PAGEDOWN|FORWARD_DELETE)/ =~ autogen then
      key = $1
      extrakey = getextrakey(key)
      list << autogen.gsub(/FROMKEYCODE_#{key}\s*,/, "KeyCode::#{key},")
      list << autogen.gsub(/FROMKEYCODE_#{key}\s*,/, "KeyCode::#{extrakey}, ModifierFlag::FN,")
      modify = true
    else
      list << autogen
    end
  end

  if modify then
    list = preprocess(list)
  end

  list
end

# ======================================================================
def parseautogen(name, lines, autogen_index)
  filter = Filter.new
  code_key = []
  code_consumer = []
  code_pointing = []
  code_variable = []

  simultaneouskeypresses_code = []
  simultaneouskeypresses_variable = []

  while true
    l = lines.shift
    break if l.nil?

    # --------------------------------------------------
    next if /<\/?item>/ =~ l
    next if /<name>.+?<\/name>/ =~ l
    next if /<appendix>.+?<\/appendix>/ =~ l
    next if /<sysctl>.+?<\/sysctl>/ =~ l
    next if /<baseunit>.+?<\/baseunit>/ =~ l
    next if /<default>.+?<\/default>/ =~ l
    next if /<vk_config>.+?<\/vk_config>/ =~ l

    # --------------------------------------------------
    if /<block>/ =~ l then
      block_code_key, block_code_consumer, block_code_pointing, block_code_variable, autogen_index, block_simultaneouskeypresses_code, block_simultaneouskeypresses_variable = parseautogen(name, lines, autogen_index)

      code_key << block_code_key unless block_code_key.empty?
      code_consumer << block_code_consumer unless block_code_consumer.empty?
      code_pointing << block_code_pointing unless block_code_pointing.empty?
      code_variable += block_code_variable
      simultaneouskeypresses_code << block_simultaneouskeypresses_code unless block_simultaneouskeypresses_code.empty?
      simultaneouskeypresses_variable += block_simultaneouskeypresses_variable

    elsif /<\/block>/ =~ l then
      break

    elsif filter.parse(l) then
      # do nothing

    elsif /<autogen>(.+?)<\/autogen>/ =~ l then
      autogen = $1
      if /^--(.+?)-- (.+)$/ =~ autogen then
        type = $1
        params = $2
        autogen_index += 1

        case type
        when 'SetKeyboardType'
          $outfile[:remapcode_keyboardtype] << "if (config.#{name} && ! config.notsave_passthrough) {\n"
          $outfile[:remapcode_keyboardtype] << "  keyboardType = #{params}.get();\n"
          $outfile[:remapcode_keyboardtype] << "}\n"

        when 'ShowStatusMessage'
          if (name == 'notsave_passthrough') then
            $outfile[:remapcode_refresh_remapfunc_statusmessage] << "if (config.#{name}) {\n"
          else
            $outfile[:remapcode_refresh_remapfunc_statusmessage] << "if (config.#{name} && ! config.notsave_passthrough) {\n"
          end
          $outfile[:remapcode_refresh_remapfunc_statusmessage] << "  strlcat(statusmessage, #{params} \" \", sizeof(statusmessage));\n"
          $outfile[:remapcode_refresh_remapfunc_statusmessage] << "  isStatusMessageVisible = true;\n"
          $outfile[:remapcode_refresh_remapfunc_statusmessage] << "}\n"

        when 'SimultaneousKeyPresses'
          $outfile[:keycode_vk_config] << "VK_SIMULTANEOUSKEYPRESSES_#{name}_#{autogen_index} --AUTO--\n"
          $func[:simultaneouskeypresses] << name
          simultaneouskeypresses_variable << { :name => "remap_#{autogen_index}_", :params => "KeyCode::VK_SIMULTANEOUSKEYPRESSES_#{name}_#{autogen_index}, #{params}" }
          simultaneouskeypresses_code << "remap_#{autogen_index}_.remap();"

        when 'KeyToKey'
          code_variable << ['RemapUtil::KeyToKey', "keytokey#{autogen_index}_"]
          code_key << "if (keytokey#{autogen_index}_.remap(remapParams, #{params})) break;"
          $func[:key] << name

        when 'DoublePressModifier'
          code_variable << ['DoublePressModifier', "doublepressmodifier#{autogen_index}_"]
          code_key << "if (doublepressmodifier#{autogen_index}_.remap(remapParams, #{params})) break;"
          $func[:key] << name

        when 'IgnoreMultipleSameKeyPress'
          code_variable << ['IgnoreMultipleSameKeyPress', "ignoremultiplesamekeypress#{autogen_index}_"]
          code_key << "if (ignoremultiplesamekeypress#{autogen_index}_.remap(remapParams, #{params})) break;"
          $func[:key] << name

        when 'KeyToConsumer'
          code_variable << ['RemapUtil::KeyToConsumer', "keytoconsumer#{autogen_index}_"]
          code_key << "if (keytoconsumer#{autogen_index}_.remap(remapParams, #{params})) break;"
          $func[:key] << name

        when 'KeyToPointingButton'
          code_variable << ['RemapUtil::KeyToPointingButton', "keytopointing#{autogen_index}_"]
          code_key << "if (keytopointing#{autogen_index}_.remap(remapParams, #{params})) break;"
          $func[:key] << name

        when 'KeyOverlaidModifier'
          code_variable << ['KeyOverlaidModifier', "keyoverlaidmodifier#{autogen_index}_"]
          code_key << "if (keyoverlaidmodifier#{autogen_index}_.remap(remapParams, #{params})) break;"
          $func[:key] << name

        when 'KeyOverlaidModifierWithRepeat'
          code_variable << ['KeyOverlaidModifier', "keyoverlaidmodifier#{autogen_index}_"]
          code_key << "if (keyoverlaidmodifier#{autogen_index}_.remapWithRepeat(remapParams, #{params})) break;"
          $func[:key] << name

        when 'ModifierHoldingKeyToKey'
          code_variable << ['ModifierHoldingKeyToKey', "modifierholdingkeytokey#{autogen_index}_"]
          code_key << "if (modifierholdingkeytokey#{autogen_index}_.remap(remapParams, #{params})) break;"
          $func[:key] << name

        when 'HoldingKeyToKey'
          code_variable << ['HoldingKeyToKey', "holdingkeytokey#{autogen_index}_"]
          code_key << "if (holdingkeytokey#{autogen_index}_.remap(remapParams, #{params})) break;"
          $func[:key] << name

        when 'ConsumerToKey'
          code_variable << ['RemapUtil::ConsumerToKey', "consumertokey#{autogen_index}_"]
          code_consumer << "if (consumertokey#{autogen_index}_.remap(remapParams, #{params})) break;"
          $func[:consumer] << name

        when 'ConsumerToConsumer'
          code_variable << ['RemapUtil::ConsumerToConsumer', "consumertoconsumer#{autogen_index}_"]
          code_consumer << "if (consumertoconsumer#{autogen_index}_.remap(remapParams, #{params})) break;"
          $func[:consumer] << name

        when 'PointingRelativeToScroll'
          code_variable << ['RemapUtil::PointingRelativeToScroll', "pointingrelativetoscroll#{autogen_index}_"]
          code_pointing << "if (pointingrelativetoscroll#{autogen_index}_.remap(remapParams, #{params})) break;"
          $func[:pointing] << name

        when 'PointingButtonToPointingButton'
          code_variable << ['RemapUtil::PointingButtonToPointingButton', "pointingbuttontopointingbutton#{autogen_index}_"]
          code_pointing << "if (pointingbuttontopointingbutton#{autogen_index}_.remap(remapParams, #{params})) break;"
          $func[:pointing] << name

        when 'PointingButtonToKey'
          code_variable << ['RemapUtil::PointingButtonToKey', "pointingbuttontokey#{autogen_index}_"]
          code_pointing << "if (pointingbuttontokey#{autogen_index}_.remap(remapParams, #{params})) break;"
          $func[:pointing] << name

        else
          print "%%% ERROR #{type} %%%\n#{l}\n"
          exit 1

        end
      end
    elsif /<.+?>.+?<\/.+?>/ =~ l then
      print "%%% ERROR unknown command %%%\n#{l}\n"
      exit 1
    end
  end

  # ======================================================================
  result_code_key = ''
  result_code_consumer = ''
  result_code_pointing = ''
  result_simultaneouskeypresses_code = ''

  unless code_key.empty? then
    result_code_key += "  do {\n"
    result_code_key += filter.to_code
    code_key.each do |line|
      result_code_key += "    #{line}\n"
    end
    result_code_key += "  } while (false);\n"
  end

  unless code_consumer.empty? then
    result_code_consumer += "  do {\n"
    result_code_consumer += filter.to_code
    code_consumer.each do |line|
      result_code_consumer += "    #{line}\n"
    end
    result_code_consumer += "  } while (false);\n"
  end

  unless code_pointing.empty? then
    result_code_pointing += "  do {\n"
    result_code_pointing += filter.to_code
    code_pointing.each do |line|
      result_code_pointing += "    #{line}\n"
    end
    result_code_pointing += "  } while (false);\n"
  end

  unless simultaneouskeypresses_variable.empty? then
    result_simultaneouskeypresses_code += "  do {\n"
    result_simultaneouskeypresses_code += filter.to_code
    simultaneouskeypresses_code.each do |line|
      result_simultaneouskeypresses_code += "    #{line}\n"
    end
    result_simultaneouskeypresses_code += "  } while (false);\n"
  end

  return result_code_key, result_code_consumer, result_code_pointing, code_variable, autogen_index, result_simultaneouskeypresses_code, simultaneouskeypresses_variable
end

# ----------------------------------------------------------------------
$stdin.read.scan(/<item>.+?<\/item>/m).each do |item|
  if /.*(<item>.+?<\/item>)/m =~ item then
    item = $1
  end

  name = nil

  if /<sysctl>([^\.]+?)\.(.+?)<\/sysctl>/m =~ item then
    name = "#{$1}_#{$2}"
    $outfile[:config_SYSCTL] << "SYSCTL_PROC(_keyremap4macbook_#{$1}, OID_AUTO, #{$2}, CTLTYPE_INT|CTLFLAG_RW, &(config.#{name}), 0, refresh_remapfunc_handler, \"I\", \"\");\n"
    $outfile[:config_register] << "sysctl_register_oid(&sysctl__keyremap4macbook_#{name});\n"
    $outfile[:config_unregister] << "sysctl_unregister_oid(&sysctl__keyremap4macbook_#{name});\n"
  end

  next if name.nil?

  $outfile[:config] << "int #{name};\n"

  if /<default>(.+?)<\/default>/m =~ item then
    $outfile[:config_default] << "#{name} = #{$1};\n"
  end

  # check <name> num == 1
  if item.scan(/<name>(.+?)<\/name>/m).size != 1 then
    print "%%% ERROR no <name> or multiple <name> for #{name}\n"
    exit 1
  end

  if /<vk_config>true<\/vk_config>/ =~ item then
    $outfile[:remapcode_vk_config] << "if (params.key == KeyCode::VK_CONFIG_TOGGLE_#{name}) {\n"
    $outfile[:remapcode_vk_config] << "  configitem = &(config.#{name});\n"
    $outfile[:remapcode_vk_config] << "  type = TYPE_TOGGLE;\n"
    $outfile[:remapcode_vk_config] << "}\n"
    $outfile[:remapcode_vk_config] << "if (params.key == KeyCode::VK_CONFIG_FORCE_ON_#{name}) {\n"
    $outfile[:remapcode_vk_config] << "  configitem = &(config.#{name});\n"
    $outfile[:remapcode_vk_config] << "  type = TYPE_FORCE_ON;\n"
    $outfile[:remapcode_vk_config] << "}\n"
    $outfile[:remapcode_vk_config] << "if (params.key == KeyCode::VK_CONFIG_FORCE_OFF_#{name}) {\n"
    $outfile[:remapcode_vk_config] << "  configitem = &(config.#{name});\n"
    $outfile[:remapcode_vk_config] << "  type = TYPE_FORCE_OFF;\n"
    $outfile[:remapcode_vk_config] << "}\n"

    $outfile[:keycode_vk_config] << "VK_CONFIG_TOGGLE_#{name} --AUTO--\n"
    $outfile[:keycode_vk_config] << "VK_CONFIG_FORCE_ON_#{name} --AUTO--\n"
    $outfile[:keycode_vk_config] << "VK_CONFIG_FORCE_OFF_#{name} --AUTO--\n"
  end

  # ======================================================================
  lines = item.split(/\n/)
  lines = preprocess(lines)

  code_key, code_consumer, code_pointing, code_variable, autogen_index, simultaneouskeypresses_code, simultaneouskeypresses_variable = parseautogen(name, lines, 0)

  unless lines.empty? then
    print "%%% ERROR no </block> at #{name} %%%\n"
  end

  next if code_key.empty? &&
    code_consumer.empty? &&
    code_pointing.empty? &&
    code_variable.empty? &&
    simultaneouskeypresses_code.empty? &&
    simultaneouskeypresses_variable.empty?

  $outfile[:remapcode_func] << "class RemapClass_#{name} {\n"
  $outfile[:remapcode_func] << "public:\n"
  unless code_key.empty? then
    $outfile[:remapcode_func] << "static void remap_key(RemapParams &remapParams) {\n"
    $outfile[:remapcode_func] << code_key
    $outfile[:remapcode_func] << "}\n"
  end
  unless code_consumer.empty? then
    $outfile[:remapcode_func] << "static void remap_consumer(RemapConsumerParams &remapParams) {\n"
    $outfile[:remapcode_func] << code_consumer
    $outfile[:remapcode_func] << "}\n"
  end
  unless code_pointing.empty? then
    $outfile[:remapcode_func] << "static void remap_pointing(RemapPointingParams_relative &remapParams) {\n"
    $outfile[:remapcode_func] << code_pointing
    $outfile[:remapcode_func] << "}\n"
  end
  $outfile[:remapcode_func] << "\n"
  $outfile[:remapcode_func] << "private:\n"
  code_variable.each do |variable|
    $outfile[:remapcode_func] << "  static #{variable[0]} #{variable[1]};\n"
  end
  $outfile[:remapcode_func] << "};\n"

  code_variable.each do |variable|
    $outfile[:remapcode_func] << "#{variable[0]} RemapClass_#{name}::#{variable[1]};\n"
  end
  $outfile[:remapcode_func] << "\n\n"

  unless simultaneouskeypresses_variable.empty? then
    $outfile[:remapcode_simultaneouskeypresses_func] << "class RemapClass_#{name} : public KeyEventInputQueue::RemapClass {\n"
    $outfile[:remapcode_simultaneouskeypresses_func] << "public:\n"

    $outfile[:remapcode_simultaneouskeypresses_func] << "void initialize(void) {\n"
    simultaneouskeypresses_variable.each do |variable|
      $outfile[:remapcode_simultaneouskeypresses_func] << "  #{variable[:name]}.initialize(#{variable[:params]});\n"
    end
    $outfile[:remapcode_simultaneouskeypresses_func] << "}\n"

    $outfile[:remapcode_simultaneouskeypresses_func] << "void remap(void) {\n"
    $outfile[:remapcode_simultaneouskeypresses_func] << simultaneouskeypresses_code
    $outfile[:remapcode_simultaneouskeypresses_func] << "}\n"

    $outfile[:remapcode_simultaneouskeypresses_func] << "bool handleVirtualKey(const Params_KeyboardEventCallBack& params) {\n"
    simultaneouskeypresses_variable.each do |variable|
      $outfile[:remapcode_simultaneouskeypresses_func] << "  if (#{variable[:name]}.handleVirtualKey(params)) return true;\n"
    end
    $outfile[:remapcode_simultaneouskeypresses_func] << "  return false;\n"
    $outfile[:remapcode_simultaneouskeypresses_func] << "}\n"

    if /^passthrough_/ =~ name then
      $outfile[:remapcode_simultaneouskeypresses_func] << "bool enabled(void) const { return config.#{name}; }\n"
    else
      $outfile[:remapcode_simultaneouskeypresses_func] << "bool enabled(void) const { return config.#{name} && ! config.notsave_passthrough; }\n"
    end

    $outfile[:remapcode_simultaneouskeypresses_func] << "private:\n"
    simultaneouskeypresses_variable.each do |variable|
      $outfile[:remapcode_simultaneouskeypresses_func] << "  KeyEventInputQueue::Remap #{variable[:name]};\n"
    end
    $outfile[:remapcode_simultaneouskeypresses_func] << "};\n"

    $outfile[:remapcode_simultaneouskeypresses_func] << "RemapClass_#{name} remapclass_#{name};\n"

    $outfile[:remapcode_simultaneouskeypresses_func] << "\n\n"
  end
end

$outfile[:remapcode_info] << "#ifndef REMAPCODE_INFO\n"
$outfile[:remapcode_info] << "#define REMAPCODE_INFO\n"
$outfile[:remapcode_info] << "enum {\n"
$outfile[:remapcode_info] << "  MAXNUM_REMAPFUNC_KEY = #{$func[:key].uniq.count},\n"
$outfile[:remapcode_info] << "  MAXNUM_REMAPFUNC_CONSUMER = #{$func[:consumer].uniq.count},\n"
$outfile[:remapcode_info] << "  MAXNUM_REMAPFUNC_POINTING = #{$func[:pointing].uniq.count},\n"
$outfile[:remapcode_info] << "};\n"
$outfile[:remapcode_info] << "#endif\n"

# set higher priority to notsave.* (ex. Vi Mode)
remapfunc_key_notsave = ''
remapfunc_key_other = ''
$func[:key].uniq.each do |name|
  text = ''
  if /^passthrough_/ =~ name then
    text += "if (config.#{name}) {\n"
  else
    text += "if (config.#{name} && ! config.notsave_passthrough) {\n"
  end
  text += "  listRemapFunc_key[listRemapFunc_key_size] = GeneratedCode::RemapClass_#{name}::remap_key;\n"
  text += "  ++listRemapFunc_key_size;\n"
  text += "}\n"
  if /^notsave_/ =~ name then
    remapfunc_key_notsave += text
  else
    remapfunc_key_other += text
  end
end
$outfile[:remapcode_refresh_remapfunc_key] << remapfunc_key_notsave
$outfile[:remapcode_refresh_remapfunc_key] << remapfunc_key_other

$func[:consumer].uniq.each do |name|
  if /^passthrough_/ =~ name then
    $outfile[:remapcode_refresh_remapfunc_consumer] << "if (config.#{name}) {\n"
  else
    $outfile[:remapcode_refresh_remapfunc_consumer] << "if (config.#{name} && ! config.notsave_passthrough) {\n"
  end
  $outfile[:remapcode_refresh_remapfunc_consumer] << "  listRemapFunc_consumer[listRemapFunc_consumer_size] = GeneratedCode::RemapClass_#{name}::remap_consumer;\n"
  $outfile[:remapcode_refresh_remapfunc_consumer] << "  ++listRemapFunc_consumer_size;\n"
  $outfile[:remapcode_refresh_remapfunc_consumer] << "}\n"
end
$func[:pointing].uniq.each do |name|
  if /^passthrough_/ =~ name then
    $outfile[:remapcode_refresh_remapfunc_pointing] << "if (config.#{name}) {\n"
  else
    $outfile[:remapcode_refresh_remapfunc_pointing] << "if (config.#{name} && ! config.notsave_passthrough) {\n"
  end
  $outfile[:remapcode_refresh_remapfunc_pointing] << "  listRemapFunc_pointing[listRemapFunc_pointing_size] = GeneratedCode::RemapClass_#{name}::remap_pointing;\n"
  $outfile[:remapcode_refresh_remapfunc_pointing] << "  ++listRemapFunc_pointing_size;\n"
  $outfile[:remapcode_refresh_remapfunc_pointing] << "}\n"
end

$outfile[:remapcode_simultaneouskeypresses_func] << "KeyEventInputQueue::RemapClass* listRemapClass_simultaneouskeypresses[] = {\n"
$func[:simultaneouskeypresses].uniq.each do |name|
  $outfile[:remapcode_simultaneouskeypresses_func] << "  &remapclass_#{name},\n"
end
$outfile[:remapcode_simultaneouskeypresses_func] << "  NULL,\n"
$outfile[:remapcode_simultaneouskeypresses_func] << "};\n"

$outfile.each do |name,file|
  file.close
end

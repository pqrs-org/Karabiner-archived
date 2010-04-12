#!/usr/bin/ruby

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
  :remapcode_refresh_remapfunc_simultaneouskeypresses => open('output/include.remapcode_refresh_remapfunc_simultaneouskeypresses.cpp', 'w'),
  :remapcode_vk_config => open('output/include.remapcode_vk_config.cpp', 'w'),
  :remapcode_simultaneouskeypresses_decl => open('output/include.remapcode_simultaneouskeypresses_decl.cpp', 'w'),
  :remapcode_simultaneouskeypresses_initialize => open('output/include.remapcode_simultaneouskeypresses_initialize.cpp', 'w'),
  :remapcode_isactive_simultaneouskeypresses => open('output/include.remapcode_isactive_simultaneouskeypresses.cpp', 'w'),
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
  filter = []
  code_key = []
  code_consumer = []
  code_pointing = []
  code_variable = []

  code_simultaneouskeypresses = []

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
      block_code_key, block_code_consumer, block_code_pointing, block_code_variable, autogen_index = parseautogen(name, lines, autogen_index)

      code_key << block_code_key unless block_code_key.empty?
      code_consumer << block_code_consumer unless block_code_consumer.empty?
      code_pointing << block_code_pointing unless block_code_pointing.empty?
      code_variable += block_code_variable

    elsif /<\/block>/ =~ l then
      break

      # ================================================================================
    elsif /<not>(.+?)<\/not>/ =~ l then
      $1.split(/,/).each do |f|
        filter << "if (remapParams.workspacedata.type == KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}) break;"
      end

    elsif /<only>(.+?)<\/only>/ =~ l then
      tmp = []
      $1.split(/,/).each do |f|
        tmp << "(remapParams.workspacedata.type != KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip})"
      end
      filter << "if (#{tmp.join(' && ')}) break;"

    elsif /<keyboardtype_not>(.+?)<\/keyboardtype_not>/ =~ l then
      $1.split(/,/).each do |f|
        filter << "if (remapParams.params.keyboardType == KeyboardType::#{f.strip}) break;"
      end

    elsif /<keyboardtype_only>(.+?)<\/keyboardtype_only>/ =~ l then
      tmp = []
      $1.split(/,/).each do |f|
        tmp << "(remapParams.params.keyboardType != KeyboardType::#{f.strip})"
      end
      filter << "if (#{tmp.join(' && ')}) break;"

    elsif /<device_not>(.+?)<\/device_not>/ =~ l then
      $1.scan(/DeviceVendorID\(.+?\)\s*,\s*DeviceProductID\(.+?\)/).each do |f|
        filter << "if (CommonData::isEqualVendorIDProductID(#{f.strip})) break;"
      end

    elsif /<device_only>(.+?)<\/device_only>/ =~ l then
      tmp = []
      $1.scan(/DeviceVendorID\(.+?\)\s*,\s*DeviceProductID\(.+?\)/).each do |f|
        tmp << "(! CommonData::isEqualVendorIDProductID(#{f.strip}))"
      end
      filter << "if (#{tmp.join(' && ')}) break;"

    elsif /<(inputmode|inputmodedetail)_not>(.+?)<\/(inputmode|inputmodedetail)_not>/ =~ l then
      inputmodetype = $1
      $2.split(/,/).each do |f|
        filter << "if (remapParams.workspacedata.#{inputmodetype} == KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip}) break;"
      end

    elsif /<(inputmode|inputmodedetail)_only>(.+?)<\/(inputmode|inputmodedetail)_only>/ =~ l then
      inputmodetype = $1
      tmp = []
      $2.split(/,/).each do |f|
        tmp << "(remapParams.workspacedata.#{inputmodetype} != KeyRemap4MacBook_bridge::GetWorkspaceData::#{f.strip})"
      end
      filter << "if (#{tmp.join(' && ')}) break;"

      # ================================================================================

    elsif /<autogen>(.+?)<\/autogen>/ =~ l then
      autogen = $1
      if /^--(.+?)-- (.+)$/ =~ autogen then
        type = $1
        params = $2
        autogen_index += 1

        case type
        when 'SetKeyboardType'
          $outfile[:remapcode_keyboardtype] << "if (config.#{name}) {\n"
          $outfile[:remapcode_keyboardtype] << "  keyboardType = #{params}.get();\n"
          $outfile[:remapcode_keyboardtype] << "}\n"

        when 'ShowStatusMessage'
          $outfile[:remapcode_refresh_remapfunc_statusmessage] << "if (config.#{name}) {\n"
          $outfile[:remapcode_refresh_remapfunc_statusmessage] << "  strlcat(statusmessage, #{params} \" \", sizeof(statusmessage));\n"
          $outfile[:remapcode_refresh_remapfunc_statusmessage] << "  isStatusMessageVisible = true;\n"
          $outfile[:remapcode_refresh_remapfunc_statusmessage] << "}\n"

        when 'SimultaneousKeyPresses'
          $outfile[:keycode_vk_config] << "VK_SIMULTANEOUSKEYPRESSES_#{name}_#{code_simultaneouskeypresses.count} --AUTO--\n"
          $func[:simultaneouskeypresses] << name
          code_simultaneouskeypresses << params

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

  unless code_key.empty? then
    result_code_key += "  do {\n"
    filter.each do |f|
      result_code_key += "    #{f}\n"
    end
    result_code_key += "\n" unless filter.empty?
    code_key.each do |line|
      result_code_key += "    #{line}\n"
    end
    result_code_key += "  } while (false);\n"
  end

  unless code_consumer.empty? then
    result_code_consumer += "  do {\n"
    filter.each do |f|
      result_code_consumer += "    #{f}\n"
    end
    result_code_consumer += "\n" unless filter.empty?
    code_consumer.each do |line|
      result_code_consumer += "    #{line}\n"
    end
    result_code_consumer += "  } while (false);\n"
  end

  unless code_pointing.empty? then
    result_code_pointing += "  do {\n"
    code_pointing.each do |line|
      result_code_pointing += "    #{line}\n"
    end
    result_code_pointing += "  } while (false);\n"
  end

  unless code_simultaneouskeypresses.empty? then
    code_simultaneouskeypresses.each_with_index do |line, index|
      $outfile[:remapcode_simultaneouskeypresses_decl] << "KeyEventInputQueue::Remap remap_#{name}_#{index};\n"
      $outfile[:remapcode_simultaneouskeypresses_initialize] << "remap_#{name}_#{index}.initialize(KeyCode::VK_SIMULTANEOUSKEYPRESSES_#{name}_#{index}, #{line});\n"

      $outfile[:remapcode_refresh_remapfunc_simultaneouskeypresses] << "if (config.#{name}) {\n"
      $outfile[:remapcode_refresh_remapfunc_simultaneouskeypresses] << "  listRemap[listRemap_size] = &remap_#{name}_#{index};\n"
      $outfile[:remapcode_refresh_remapfunc_simultaneouskeypresses] << "  ++listRemap_size;\n"
      $outfile[:remapcode_refresh_remapfunc_simultaneouskeypresses] << "}\n"
    end

    $outfile[:remapcode_isactive_simultaneouskeypresses] << "if (config.#{name}) config.set_isactive_simultaneouskeypresses(true);\n"
  end

  return result_code_key, result_code_consumer, result_code_pointing, code_variable, autogen_index
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

  # ======================================================================
  lines = item.split(/\n/)
  lines = preprocess(lines)

  code_key, code_consumer, code_pointing, code_variable, autogen_index = parseautogen(name, lines, 0)

  unless lines.empty? then
    print "%%% ERROR no </block> at #{name} %%%\n"
  end

  next if code_key.empty? &&
    code_consumer.empty? &&
    code_pointing.empty? &&
    code_variable.empty?

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
end

$outfile[:remapcode_info] << "#ifndef REMAPCODE_INFO\n"
$outfile[:remapcode_info] << "#define REMAPCODE_INFO\n"
$outfile[:remapcode_info] << "enum {\n"
$outfile[:remapcode_info] << "  MAXNUM_REMAPFUNC_KEY = #{$func[:key].uniq.count},\n"
$outfile[:remapcode_info] << "  MAXNUM_REMAPFUNC_CONSUMER = #{$func[:consumer].uniq.count},\n"
$outfile[:remapcode_info] << "  MAXNUM_REMAPFUNC_POINTING = #{$func[:pointing].uniq.count},\n"
$outfile[:remapcode_info] << "  MAXNUM_REMAPFUNC_SIMULTANEOUSKEYPRESSES = #{$func[:simultaneouskeypresses].count},\n" # don't uniq for simultaneouskeypresses
$outfile[:remapcode_info] << "};\n"
$outfile[:remapcode_info] << "#endif\n"

# set higher priority to notsave.* (ex. Vi Mode)
remapfunc_key_notsave = ''
remapfunc_key_other = ''
$func[:key].uniq.each do |name|
  text = ''
  text += "if (config.#{name}) {\n"
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
  $outfile[:remapcode_refresh_remapfunc_consumer] << "if (config.#{name}) {\n"
  $outfile[:remapcode_refresh_remapfunc_consumer] << "  listRemapFunc_consumer[listRemapFunc_consumer_size] = GeneratedCode::RemapClass_#{name}::remap_consumer;\n"
  $outfile[:remapcode_refresh_remapfunc_consumer] << "  ++listRemapFunc_consumer_size;\n"
  $outfile[:remapcode_refresh_remapfunc_consumer] << "}\n"
end
$func[:pointing].uniq.each do |name|
  $outfile[:remapcode_refresh_remapfunc_pointing] << "if (config.#{name}) {\n"
  $outfile[:remapcode_refresh_remapfunc_pointing] << "  listRemapFunc_pointing[listRemapFunc_pointing_size] = GeneratedCode::RemapClass_#{name}::remap_pointing;\n"
  $outfile[:remapcode_refresh_remapfunc_pointing] << "  ++listRemapFunc_pointing_size;\n"
  $outfile[:remapcode_refresh_remapfunc_pointing] << "}\n"
end

$outfile.each do |name,file|
  file.close
end

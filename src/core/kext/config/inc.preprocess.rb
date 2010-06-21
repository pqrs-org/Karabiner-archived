#!/usr/bin/ruby
# -*- coding: utf-8 -*-

class Preprocesser
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
  protected :getextrakey

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
end

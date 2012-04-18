#!/usr/bin/ruby

require "#{File.dirname(__FILE__)}/../lib/converter.rb"

[
 {
   :filename => '../../output/include.bridge_essential_configuration_enum.h.tmp',
   :parameter => 'output_bridge_essential_configuration_enum_h',
 },
 {
   :filename => '../../output/include.bridge_essential_configuration_default_values.c.tmp',
   :parameter => 'output_bridge_essential_configuration_default_values_c',
 },
 {
   :filename => '../../output/include.bridge_essential_configuration_identifiers.m.tmp',
   :parameter => 'output_bridge_essential_configuration_identifiers_m',
 },
].each do |info|
  dump_xml_compiler_result = '../../../bin/dump_xml_compiler_result/build/Release/dump_xml_compiler_result'
  system_xml_directory     = '../../../core/server/Resources'
  private_xml_directory    = '../../../core/server/Resources'
  system("#{dump_xml_compiler_result} #{system_xml_directory} #{private_xml_directory} #{info[:parameter]} > #{info[:filename]}")

  KeyRemap4MacBookBridge::Converter.update_file_if_needed(info[:filename])
end

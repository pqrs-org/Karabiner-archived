#!/usr/bin/ruby

xmlfile = ARGV[0]
if xmlfile.nil? || ! FileTest.exist?(xmlfile) then
  $stderr.print "Usage: conv.rb checkbox.xml\n"
  exit 1
end

open(xmlfile) do |f|
  lines = f.readlines

  while true
    isreplaced = false
    result = []

    lines.each do |l|
      if /<!--\s*#include "(.+)"\s*-->/ =~ l then
        includefile = $1
        unless /^\|/ =~ includefile then
          unless FileTest.exist?(includefile) then
            $stderr.print "No such file: #{includefile}\n"
            exit 1
          end
        end

        open(includefile) do |infile|
          result += infile.readlines
        end
        isreplaced = true
      else
        result << l
      end
    end

    lines = result

    break unless isreplaced
  end

  outputdir = File.dirname($0) + '/../output'
  if ! FileTest.exist?(outputdir) then
    Dir.mkdir(outputdir)
  end

  open("#{outputdir}/checkbox.xml", "w") do |outfile|
    outfile.print lines.join('')
  end
end

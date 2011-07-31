#!/usr/bin/ruby

system('make -C ../../files/prefpane > /dev/null')
file = "%s/../../files/prefpane/output/checkbox.xml" % File.dirname($0)
xml = File.read(file)

version = IO.read('../../version')

total = xml.scan(/<identifier(\s.*?)?>(.+?)<\/identifier>/m).count
print "SUBJECT Version #{version.strip} (Total: #{total} prefs)\n"
print "RAW\n"

$first = true

xml.scan(/<(.+?)>([^<]*)/m).each do |matches|
  name = matches[0]
  body = matches[1]

  case name
  when 'list'
    if $first then
      print '<ul id="collapser">'
      $first = false
    else
      print '<ul>'
    end

  when '/list'
    print "</ul>\n"

  when 'item'
    print '<li>'

  when '/item'
    print '</li>'

  when 'name'
    print body
    print "<br/>"

  when 'appendix'
    print "&nbsp;&nbsp;#{body}<br/>"

  end
end

print "/RAW\n"

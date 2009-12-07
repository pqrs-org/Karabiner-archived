#!/bin/sh

for f in \
    /System/Library/Input\ Methods/*/Contents/Info.plist \
    /Library/Input\ Methods/*/Contents/Info.plist \
    ; do
    echo '----------------------------------------'
    echo $f
    echo
    rubycode='
l = readlines.join("\n")
l.scan(/<key>tsVisibleInputModeOrderedArrayKey<\/key>.*?<array>(.*?)<\/array>/m) do |text|
  text[0].scan(/<string>(.+?)<\/string>/) do |name|
    print name, "\n"
  end
end'
    ruby -e "$rubycode" "$f"
done

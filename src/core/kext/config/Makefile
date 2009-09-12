SYSCTL_FILE = ../../../../files/prefpane/checkbox.xml ../../../../files/prefpane/number.xml

all:
	mkdir -p output
	/usr/bin/tidy -q -errors -xml $(SYSCTL_FILE)
	cat $(SYSCTL_FILE) | ruby ./make-code.rb

clean:
	rm -f output/*

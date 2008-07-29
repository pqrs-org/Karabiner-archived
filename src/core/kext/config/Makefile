SYSCTL_FILE = ../../../../files/prefpane/checkbox.xml ../../../../files/prefpane/number.xml

all:
	mkdir -p output
	cat $(SYSCTL_FILE) | ruby ./make-code.rb

clean:
	rm -f output/*

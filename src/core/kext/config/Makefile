SYSCTL_FILE = ../../../../files/prefpane/output/checkbox.xml ../../../../files/prefpane/number.xml

all:
	mkdir -p output
	mkdir -p ../keycode/data/include
	make -C ../../../../files/prefpane
	/usr/bin/tidy -q -errors -xml -raw $(SYSCTL_FILE)
	cat $(SYSCTL_FILE) | ruby ./make-code.rb

clean:
	rm -f output/*

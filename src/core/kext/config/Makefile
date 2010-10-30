SYSCTL_FILE = ../../../../files/prefpane/output/checkbox.xml ../../../../files/prefpane/number.xml

all:
	mkdir -p output
	make -C ../../../../files/prefpane
	/usr/bin/tidy -q -errors -xml -raw $(SYSCTL_FILE)
	make -C ../../bridge/keycode
	ruby ./make-code.rb $(SYSCTL_FILE)

clean:
	rm -f output/*

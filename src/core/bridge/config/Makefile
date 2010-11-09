SYSCTL_FILE = ../../../../files/prefpane/number.xml ../../../../files/prefpane/output/checkbox.xml

all:
	mkdir -p output
	make -C ../../../../files/prefpane
	/usr/bin/tidy -q -errors -xml -raw $(SYSCTL_FILE)
	ruby ./make-code.rb $(SYSCTL_FILE)

clean:
	rm -f output/*

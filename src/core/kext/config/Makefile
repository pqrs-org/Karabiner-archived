SYSCTL_FILE = ../../../../files/prefpane/output/checkbox.xml ../../../../files/prefpane/number.xml

all:
	mkdir -p output
	make -C ../../../../files/prefpane
	/usr/bin/tidy -q -errors -xml -raw $(SYSCTL_FILE)
	make -C ../keycode output
	ruby ./make-code.rb $(SYSCTL_FILE)

clean:
	rm -f output/*

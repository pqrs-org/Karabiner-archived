SYSCTL_FILE = ../../../../files/prefpane/output/checkbox.xml ../../../../files/prefpane/number.xml

all:
	mkdir -p output
	mkdir -p ../keycode/data/include
	make -C ../../../../files/prefpane
	/usr/bin/tidy -q -errors -xml -raw $(SYSCTL_FILE)
	ruby ./make-keycode.rb
	ruby ./make-configindex.rb
	make -C ../keycode output
	ruby ./make-code.rb $(SYSCTL_FILE)

clean:
	rm -f output/*

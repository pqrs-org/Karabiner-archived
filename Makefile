all:
	./make-package.sh

build:
	$(MAKE) -C pkginfo
	$(MAKE) -C files/prefpane
	$(MAKE) -C src
	mkdir -p files/share

clean:
	git clean -f -x -d

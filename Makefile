all:
	./make-package.sh

build:
	$(MAKE) -C pkginfo
	$(MAKE) -C files/prefpane
	$(MAKE) -C src
	mkdir -p files/share

clean:
	$(MAKE) -C pkginfo clean
	$(MAKE) -C src clean
	$(MAKE) -C util clean
	$(MAKE) -C Tests clean
	rm -rf pkgroot
	rm -rf *.pkg
	rm -rf *.zip

source:
	./make-source.sh

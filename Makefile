all:
	./make-package.sh

build:
	$(MAKE) -C pkginfo
	$(MAKE) -C files/prefpane
	$(MAKE) -C src
	mkdir -p files/share

clean:
	$(MAKE) -C src clean
	$(MAKE) -C util clean
	$(MAKE) -C Tests clean
	sudo rm -rf pkgroot
	sudo rm -rf *.pkg
	sudo rm -rf *.zip

source:
	./make-source.sh

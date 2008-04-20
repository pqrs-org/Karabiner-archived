all:
	./make-package.sh

build:
	$(MAKE) -C pkginfo
	$(MAKE) -C src/kext
	$(MAKE) -C src/server
	$(MAKE) -C prefpane

clean:
	$(MAKE) -C src/kext clean
	$(MAKE) -C src/server clean
	$(MAKE) -C prefpane clean
	sudo rm -rf pkgroot
	sudo rm -rf *.pkg
	sudo rm -rf *.tar.gz

source:
	./make-source.sh

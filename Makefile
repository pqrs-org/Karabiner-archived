all:
	./make-package.sh

build:
	$(MAKE) -C pkginfo
	$(MAKE) -C files/prefpane
	$(MAKE) -C src
	mkdir -p files/share
	./util/make-reset.rb files/prefpane/*.xml > files/share/reset

clean:
	$(MAKE) -C src clean
	$(MAKE) -C util clean
	sudo rm -rf pkgroot
	sudo rm -rf *.pkg
	sudo rm -rf *.zip

source:
	./make-source.sh

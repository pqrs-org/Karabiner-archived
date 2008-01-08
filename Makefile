all:
	./make-package.sh

build:
	$(MAKE) -C src/kext
	$(MAKE) -C pkginfo

clean:
	$(MAKE) -C src/kext clean
	sudo rm -rf pkgroot
	sudo rm -rf *.pkg
	sudo rm -rf *.pkg.tar.gz

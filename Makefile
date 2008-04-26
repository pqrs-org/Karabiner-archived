all:
	./make-package.sh

build:
	$(MAKE) -C pkginfo
	$(MAKE) -C src/kext
	$(MAKE) -C src/server
	$(MAKE) -C src/prefpane
	$(MAKE) -C src/sysctl_confd
	$(MAKE) -C src/sysctl_reset
	$(MAKE) -C src/sysctl_save
	$(MAKE) -C src/sysctl_set
	mkdir -p files/share
	./util/make-reset.rb files/prefpane/*.xml > files/share/reset

clean:
	$(MAKE) -C src/kext clean
	$(MAKE) -C src/server clean
	$(MAKE) -C src/prefpane clean
	$(MAKE) -C src/sysctl_confd clean
	$(MAKE) -C src/sysctl_reset clean
	$(MAKE) -C src/sysctl_save clean
	$(MAKE) -C src/sysctl_set clean
	sudo rm -rf pkgroot
	sudo rm -rf *.pkg
	sudo rm -rf *.tar.gz

source:
	./make-source.sh

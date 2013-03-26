all:
	./make-package.sh

build:
	$(MAKE) -C pkginfo
	$(MAKE) -C src
	$(MAKE) -C files/Utilities

clean:
	git clean -f -x -d

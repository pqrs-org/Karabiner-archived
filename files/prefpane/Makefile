all:
	./scripts/conv.rb checkbox.xml
	./scripts/lint.rb

install: all
	make -C ../../src/core/server/Resources install

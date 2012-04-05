all:
	./scripts/conv.rb checkbox.xml
	./scripts/lint.rb

install: all
	sudo install -m 644 number.xml output/checkbox.xml /Library/org.pqrs/KeyRemap4MacBook/prefpane
	sudo install -m 644 number.xml output/checkbox.xml /Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook.app/Contents/Resources

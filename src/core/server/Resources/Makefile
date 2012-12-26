all:
	@echo "check xml"
	for f in `find * -name '*.xml'`; do \
		echo $$f; \
		tidy -qe -xml -raw $$f; \
		if [ $$? == 2 ]; then \
			exit 2; \
		 fi; \
	done

install:
	sudo cp -r *.xml include /Applications/KeyRemap4MacBook.app/Contents/Resources
	sudo /Applications/KeyRemap4MacBook.app/Contents/Library/extra/setpermissions.sh /Applications/KeyRemap4MacBook.app/Contents/Resources

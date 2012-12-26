all:
	@for f in `find * -name '*.xml'`; do \
		echo "check $$f"; \
		tidy -qe -xml -raw $$f 2>&1; \
		if [ $$? != 0 ]; then \
			exit 2; \
		fi; \
	done

install: all
	sudo cp -r *.xml include /Applications/KeyRemap4MacBook.app/Contents/Resources
	sudo /Applications/KeyRemap4MacBook.app/Contents/Library/extra/setpermissions.sh /Applications/KeyRemap4MacBook.app/Contents/Resources

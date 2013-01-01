all:
	@for f in `find * -name '*.xml'`; do \
		echo "check $$f"; \
		/usr/bin/tidy -qe -xml -raw $$f 2>&1; \
		if [ $$? != 0 ]; then \
			exit 1; \
		fi; \
		/usr/bin/grep 'private\.' $$f; \
		if [ $$? != 1 ]; then \
			echo '"private." is matched.'; \
			exit 1; \
		fi; \
	done

install: all
	sudo cp -r *.xml include /Applications/KeyRemap4MacBook.app/Contents/Resources
	sudo /Applications/KeyRemap4MacBook.app/Contents/Library/extra/setpermissions.sh /Applications/KeyRemap4MacBook.app/Contents/Resources
	/Applications/KeyRemap4MacBook.app/Contents/Applications/KeyRemap4MacBook_cli.app/Contents/MacOS/KeyRemap4MacBook_cli reloadxml

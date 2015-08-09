all:
	@for f in `find * -name '*.xml'`; do \
		echo "check $$f"; \
		/usr/bin/tidy -qe -xml -raw $$f 2>&1; \
		if [ $$? != 0 ]; then \
			exit 1; \
		fi; \
		/usr/bin/grep -E '(>[[:space:]]*private\.)|(^[[:space:]]*private\.)' $$f; \
		if [ $$? != 1 ]; then \
			echo '"private." is matched.'; \
			exit 1; \
		fi; \
		/usr/bin/grep -e '--[A-Z]' $$f; \
		if [ $$? != 1 ]; then \
			echo 'Using --KeyToKey-- style autogen. Replace --KeyToKey-- with __KeyToKey__.'; \
			exit 1; \
		fi; \
	done

install: all
	sudo cp -r *.xml include /Applications/Karabiner.app/Contents/Resources
	sudo ../../../../files/extra/setpermissions.sh /Applications/Karabiner.app/Contents/Resources
	/Applications/Karabiner.app/Contents/Library/bin/karabiner reloadxml

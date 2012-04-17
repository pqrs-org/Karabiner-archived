all:
	mkdir -p ../../output
	make -C ../keycode/
	make -C ../../../bin/dump_xml_compiler_result
	ruby ./make-code.rb

clean:
	rm -f ../../output/*

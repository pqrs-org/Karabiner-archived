all:
	make -C ../config prepare
	make output

output:
	mkdir -p output
	ruby ./make-code.rb

clean:
	rm -f output/*

.PHONY: output

all:
	make -C ../config
	make output

output:
	mkdir -p output
	ruby ./make-code.rb

clean:
	rm -f output/*

.PHONY: output

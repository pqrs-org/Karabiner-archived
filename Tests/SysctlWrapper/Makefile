CFLAGS += -fobjc-gc -Wall -Werror -Wmissing-prototypes -Wreturn-type -Wunused-variable -Wshorten-64-to-32

SOURCES=$(wildcard *.m)
OBJECTS=$(patsubst %.m,%.o,$(SOURCES))

all: $(OBJECTS)
	g++ -o test $(OBJECTS) -framework cocoa

clean:
	rm -f *.o test

CFLAGS += -fobjc-gc -Wall -Werror -Wmissing-prototypes -Wreturn-type -Wunused-variable -Wshorten-64-to-32

all: getActiveApplicationName.o main.o
	g++ -o getname getActiveApplicationName.o main.o -framework cocoa

clean:
	rm -f *.o getname

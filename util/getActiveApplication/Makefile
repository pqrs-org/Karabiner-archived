CFLAGS += -Wall -Werror -O2 -fobjc-gc
CXXFLAGS += -Wall -Werror -O2
OBJECTS += main.o server_objc_part.o

all: $(OBJECTS)
	g++ -o getname $(OBJECTS) -framework cocoa -framework carbon

clean:
	rm -f *.o getname

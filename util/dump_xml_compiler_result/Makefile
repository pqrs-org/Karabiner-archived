CXXFLAGS += -I../../src/lib/xml_compiler/include -std=c++11

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(SOURCES))

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o dump_xml_compiler_result $(SOURCES) $(LDFLAGS) \
	../../src/lib/xml_compiler/build/Release/libxml_compiler.a

clean:
	rm -f *.o a.out

include ../../Makefile.common

CXXFLAGS += -I../../../src/lib/xml_compiler/include -std=c++11

include ../../Makefile.rules

a.out: $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES) $(LDFLAGS) \
	../../../src/lib/xml_compiler/build/Release/libxml_compiler.a

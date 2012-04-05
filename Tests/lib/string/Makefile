include ../../Makefile.common

CXXFLAGS += -I../../../src/lib/xml_compiler/include

a.out: $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES) $(LDFLAGS) ../../../src/lib/xml_compiler/build/Release/libxml_compiler.a

include ../../Makefile.rules

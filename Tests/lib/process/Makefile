include ../../Makefile.common

CXXFLAGS += -I../../../src/lib/xml_compiler/include -std=c++11 -fvisibility=hidden

include ../../Makefile.rules

a.out: $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES) $(LDFLAGS)

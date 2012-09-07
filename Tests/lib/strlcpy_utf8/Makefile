include ../../Makefile.common

CXXFLAGS += -I../../../src/lib/strlcpy_utf8 -std=c++11 -fvisibility=hidden

include ../../Makefile.rules

a.out: $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES) $(LDFLAGS)

include ../../Makefile.rules

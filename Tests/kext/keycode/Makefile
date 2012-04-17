include ../../Makefile.common

a.out: $(SOURCES)
	$(MAKE) -C ../../../src/bridge/generator/config
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES) $(LDFLAGS)

include ../../Makefile.rules

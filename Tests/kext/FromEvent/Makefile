include ../../Makefile.common
CXXFLAGS += -DBUTTONSTATUS_TEST -I../../../src/bridge/include

a.out: $(SOURCES)
	$(MAKE) -C ../../../src/bridge/generator/config
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES) $(LDFLAGS)

include ../../Makefile.rules

include ../../Makefile.common
CXXFLAGS += -DBUTTONSTATUS_TEST -I../bridge

a.out: $(SOURCES)
	$(MAKE) -C ../bridge
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES) $(LDFLAGS)

include ../../Makefile.rules

include ../../Makefile.common
CXXFLAGS += -DFLAGSTATUS_TEST

a.out: $(SOURCES)
	$(MAKE) -C ../bridge/keycode
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES) $(LDFLAGS)

include ../../Makefile.rules

include ../../Makefile.common

a.out: $(SOURCES)
	$(MAKE) -C ../bridge/keycode
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES) $(LDFLAGS)

include ../../Makefile.rules

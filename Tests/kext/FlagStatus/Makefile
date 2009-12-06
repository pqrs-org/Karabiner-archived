include ../../Makefile.common

a.out: $(SOURCES)
	$(MAKE) -C keycode
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES) $(LDFLAGS)

include ../../Makefile.rules

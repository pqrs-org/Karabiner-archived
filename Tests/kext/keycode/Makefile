include ../../Makefile.common

a.out: $(SOURCES)
	$(MAKE) -C ../bridge
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES) $(LDFLAGS)

include ../../Makefile.rules

include ../../Makefile.common

CXXFLAGS += -I../../../src/lib/xml_compiler/include -I../../../src/bridge/include -std=c++11 -fvisibility=hidden

include ../../Makefile.rules

a.out: $(SOURCES)
	make -C ../../../src/lib/xml_compiler
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES) $(LDFLAGS) \
	../../../src/lib/xml_compiler/build/Release/libxml_compiler.a

clean::
	make -C ../../../src/lib/xml_compiler clean

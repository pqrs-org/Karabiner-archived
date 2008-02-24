SYSCTL_FILE = ../../../files/prefpane/sysctl.xml

all:
	mkdir -p output
	cat $(SYSCTL_FILE) | ruby ./make-code.rb hpp > output/include.config.hpp
	cat $(SYSCTL_FILE) | ruby ./make-code.rb cpp_SYSCTL > output/include.config_SYSCTL.cpp
	cat $(SYSCTL_FILE) | ruby ./make-code.rb cpp_register > output/include.config_register.cpp
	cat $(SYSCTL_FILE) | ruby ./make-code.rb cpp_unregister > output/include.config_unregister.cpp

clean:
	rm -f output/*

SYSCTL_FILE = ../../../../files/prefpane/checkbox.xml ../../../../files/prefpane/number.xml

all:
	mkdir -p output
	cat $(SYSCTL_FILE) | ruby ./make-code.rb hpp > output/include.config.hpp
	cat $(SYSCTL_FILE) | ruby ./make-code.rb default > output/include.config.default.hpp
	cat $(SYSCTL_FILE) | ruby ./make-code.rb cpp_SYSCTL > output/include.config_SYSCTL.cpp
	cat $(SYSCTL_FILE) | ruby ./make-code.rb cpp_register > output/include.config_register.cpp
	cat $(SYSCTL_FILE) | ruby ./make-code.rb cpp_unregister > output/include.config_unregister.cpp
	cat $(SYSCTL_FILE) | ruby ./make-code.rb remapcode_func > output/include.remapcode_func.cpp
	cat $(SYSCTL_FILE) | ruby ./make-code.rb remapcode_call > output/include.remapcode_call.cpp
	cat $(SYSCTL_FILE) | ruby ./make-code.rb remapcode_call_kom > output/include.remapcode_call_kom.cpp
	cat $(SYSCTL_FILE) | ruby ./make-code.rb remapcode_call_komc > output/include.remapcode_call_komc.cpp
	cat $(SYSCTL_FILE) | ruby ./make-code.rb remapcode_call_consumer > output/include.remapcode_call_consumer.cpp
	cat $(SYSCTL_FILE) | ruby ./make-code.rb remapcode_call_pointing_relative > output/include.remapcode_call_pointing_relative.cpp

clean:
	rm -f output/*

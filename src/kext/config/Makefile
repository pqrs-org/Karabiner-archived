all:
	mkdir -p output
	ruby ./make-code.rb hpp > output/include.config.hpp
	ruby ./make-code.rb cpp_SYSCTL > output/include.config_SYSCTL.cpp
	ruby ./make-code.rb cpp_register > output/include.config_register.cpp
	ruby ./make-code.rb cpp_unregister > output/include.config_unregister.cpp

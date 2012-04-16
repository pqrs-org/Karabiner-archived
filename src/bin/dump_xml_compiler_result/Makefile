#CONFIGURATION = Debug
CONFIGURATION = Release

all:
	mkdir -p build/Release
	$(MAKE) -C ../../../pkginfo
	$(MAKE) -C ../../lib/xml_compiler
	xcodebuild -alltargets -configuration $(CONFIGURATION) build

clean:
	rm -rf build
	rm -f Info.plist
	rm -f *.xcodeproj/*.mode1
	rm -f *.xcodeproj/*.mode1v3
	rm -f *.xcodeproj/*.pbxuser
	rm -rf *.xcodeproj/xcuserdata
	rm -rf *.xcodeproj/project.xcworkspace/xcuserdata
	make -C ../../lib/xml_compiler clean

xcode:
	open *.xcodeproj

dump_data:
	@./build/Release/dump_xml_compiler_result \
	/Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook.app/Contents/Resources \
	/noexistspath \
	dump_data

dump_tree:
	@./build/Release/dump_xml_compiler_result \
	/Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook.app/Contents/Resources \
	/noexistspath \
	dump_tree

dump_number:
	@./build/Release/dump_xml_compiler_result \
	/Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook.app/Contents/Resources \
	/noexistspath \
	dump_number

dump_identifier_except_essential:
	@./build/Release/dump_xml_compiler_result \
	/Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook.app/Contents/Resources \
	/noexistspath \
	dump_identifier_except_essential

output_bridge_essential_config_index_hpp:
	@./build/Release/dump_xml_compiler_result \
	/Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook.app/Contents/Resources \
	/noexistspath \
	output_bridge_essential_config_index_hpp

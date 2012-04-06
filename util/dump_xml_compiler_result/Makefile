#CONFIGURATION = Debug
CONFIGURATION = Release

all:
	$(MAKE) -C ../../pkginfo
	$(MAKE) -C ../../src/lib/xml_compiler
	xcodebuild -alltargets -configuration $(CONFIGURATION) build

clean:
	rm -rf build
	rm -f Info.plist
	rm -f *.xcodeproj/*.mode1
	rm -f *.xcodeproj/*.mode1v3
	rm -f *.xcodeproj/*.pbxuser
	rm -rf *.xcodeproj/xcuserdata
	rm -rf *.xcodeproj/project.xcworkspace/xcuserdata
	make -C ../../src/lib/xml_compiler clean

xcode:
	open *.xcodeproj

dump_data:
	@./build/Release/dump_xml_compiler_result.app/Contents/MacOS/dump_xml_compiler_result

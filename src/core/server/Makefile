#CONFIGURATION = Debug
CONFIGURATION = Release

all:
	$(MAKE) -C Resources
	$(MAKE) -C ../../../pkginfo
	$(MAKE) -C ../../bridge/generator/config
	$(MAKE) -C ../../lib/xml_compiler
	xcodebuild -alltargets -configuration $(CONFIGURATION) build

buildonly:
	xcodebuild -alltargets -configuration $(CONFIGURATION) build

clean:
	rm -rf build
	rm -f Info.plist
	rm -f *.xcodeproj/*.mode1
	rm -f *.xcodeproj/*.mode1v3
	rm -f *.xcodeproj/*.pbxuser
	rm -rf *.xcodeproj/xcuserdata
	rm -rf *.xcodeproj/project.xcworkspace/xcuserdata
	$(MAKE) -C ../../lib/xml_compiler clean

xcode:
	open *.xcodeproj

run: all
	./build/Release/KeyRemap4MacBook.app/Contents/MacOS/KeyRemap4MacBook

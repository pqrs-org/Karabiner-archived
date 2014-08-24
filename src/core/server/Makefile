#CONFIGURATION = Debug
CONFIGURATION = Release

all:
	pod install --no-repo-update
	$(MAKE) -C Resources
	$(MAKE) -C ../../../pkginfo
	$(MAKE) -C ../../bridge/generator/bridge_version
	$(MAKE) -C ../../bridge/generator/config
	$(MAKE) -C ../../lib/xml_compiler
	xcodebuild \
		-workspace *.xcworkspace \
		-scheme Karabiner_server \
		-configuration $(CONFIGURATION) \
		build \
		SYMROOT=`pwd`/build

buildonly:
	xcodebuild \
		-workspace *.xcworkspace \
		-scheme Karabiner_server \
		-configuration $(CONFIGURATION) \
		build \
		SYMROOT=`pwd`/build

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
	open *.xcworkspace

run: all
	open ./build/Release/Karabiner.app

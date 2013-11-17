#CONFIGURATION = Debug
CONFIGURATION = Release
VERSION = `cat ../../../version`

all:
	$(MAKE) -C ../../../pkginfo
	$(MAKE) -C ../../bridge/generator/bridge_version
	$(MAKE) -C ../../bridge/generator/config
	xcodebuild -alltargets -configuration $(CONFIGURATION) MODULE_VERSION=$(VERSION) CURRENT_PROJECT_VERSION=$(VERSION) build

clean:
	rm -rf build
	rm -f Info.plist
	rm -f *.xcodeproj/*.mode1
	rm -f *.xcodeproj/*.mode1v3
	rm -f *.xcodeproj/*.pbxuser
	rm -rf *.xcodeproj/xcuserdata
	rm -rf *.xcodeproj/project.xcworkspace/xcuserdata

xcode:
	open *.xcodeproj

reload:
	sudo ./scripts/unload.sh
	sudo ./scripts/load.sh
	@./scripts/wait.sh

checkstackusage:
	sh -c 'for f in build/KeyRemap4MacBook.build/Release/KeyRemap4MacBook.build/Objects-normal/i386/*.o; do echo $$f; otool -Vt $$f |grep esp |grep sub |grep 0x00; done; exit 0'
	sh -c 'for f in build/KeyRemap4MacBook.build/Release/KeyRemap4MacBook.build/Objects-normal/x86_64/*.o; do echo $$f; otool -Vt $$f |grep rsp |grep sub |grep 0x00; done; exit 0'

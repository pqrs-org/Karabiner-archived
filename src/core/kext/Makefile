#CONFIGURATION = Debug
CONFIGURATION = Release
VERSION = `cat ../../../version`

all:
	$(MAKE) -C ../../../pkginfo
	echo "char config_version[] = \"$(VERSION)\";" > version.hpp
	$(MAKE) -C ../bridge
	$(MAKE) -C config
	xcodebuild -alltargets -configuration $(CONFIGURATION) MODULE_VERSION=$(VERSION) CURRENT_PROJECT_VERSION=$(VERSION) build

buildonly:
	xcodebuild -alltargets -configuration $(CONFIGURATION) MODULE_VERSION=$(VERSION) CURRENT_PROJECT_VERSION=$(VERSION) build

clean:
	$(MAKE) -C config clean
	rm -rf build
	rm -f *.xcodeproj/*.mode1
	rm -f *.xcodeproj/*.mode1v3
	rm -f *.xcodeproj/*.pbxuser

reload:
	sudo ./scripts/unload.sh
	sudo ./scripts/load.sh
	@./scripts/wait.sh

checkstackusage:
	sh -c 'for f in build/KeyRemap4MacBook.build/Release/KeyRemap4MacBook.build/Objects-normal/i386/*.o; do echo $$f; otool -Vt $$f |grep esp |grep sub |grep 0x00; done; exit 0'
	sh -c 'for f in build/KeyRemap4MacBook.build/Release/KeyRemap4MacBook.build/Objects-normal/x86_64/*.o; do echo $$f; otool -Vt $$f |grep rsp |grep sub |grep 0x00; done; exit 0'

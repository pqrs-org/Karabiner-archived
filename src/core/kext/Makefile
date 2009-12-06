#CONFIGURATION = Debug
CONFIGURATION = Release
VERSION = `cat ../../../version`

all:
	$(MAKE) -C ../../../pkginfo
	echo "char config_version[] = \"$(VERSION)\";" > version.hpp
	$(MAKE) -C config
	$(MAKE) -C keycode
	xcodebuild -alltargets -configuration $(CONFIGURATION) MODULE_VERSION=$(VERSION) CURRENT_PROJECT_VERSION=$(VERSION) build

buildonly:
	xcodebuild -alltargets -configuration $(CONFIGURATION) MODULE_VERSION=$(VERSION) CURRENT_PROJECT_VERSION=$(VERSION) build

clean:
	$(MAKE) -C config clean
	$(MAKE) -C keycode clean
	rm -rf build
	rm -f *.xcodeproj/*.mode1
	rm -f *.xcodeproj/*.mode1v3
	rm -f *.xcodeproj/*.pbxuser

reload:
	sudo ./scripts/unload.sh
	sudo ./scripts/load.sh
	@./scripts/wait.sh

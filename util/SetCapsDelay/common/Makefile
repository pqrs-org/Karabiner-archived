#CONFIGURATION = Debug
CONFIGURATION = Release

all:
	$(MAKE) -C ../../../pkginfo
	xcodebuild -alltargets -configuration $(CONFIGURATION) MODULE_VERSION=`cat ../../../version` CURRENT_PROJECT_VERSION=`cat ../../../version` build

clean:
	rm -rf build
	rm -f *.xcodeproj/*.mode1
	rm -f *.xcodeproj/*.mode1v3
	rm -f *.xcodeproj/*.pbxuser

reload:
	-sudo ./scripts/unload.sh
	-sudo ./scripts/load.sh

unload:
	-sudo ./scripts/unload.sh

load:
	-sudo ./scripts/load.sh

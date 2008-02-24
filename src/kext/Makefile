#CONFIGURATION = Debug
CONFIGURATION = Release

all:
	$(MAKE) -C config
	xcodebuild -alltargets -configuration $(CONFIGURATION) build

clean:
	xcodebuild -alltargets -configuration $(CONFIGURATION) clean
	rm -rf build
	rm -f *.xcodeproj/*.mode1
	rm -f *.xcodeproj/*.mode1v3
	rm -f *.xcodeproj/*.pbxuser

reload:
	sudo ./scripts/unload.sh
	sudo ./scripts/load.sh

#CONFIGURATION = Debug
CONFIGURATION = Release
VERSION = `cat ../../../version`

all:
	$(MAKE) -C ../../../pkginfo
	echo "char config_version[] = \"$(VERSION)\";" > version.hpp
	$(MAKE) -C config
	xcodebuild -alltargets -configuration $(CONFIGURATION) MODULE_VERSION=$(VERSION) CURRENT_PROJECT_VERSION=$(VERSION) build

clean:
	$(MAKE) -C config clean
	rm -rf build
	rm -f *.xcodeproj/*.mode1
	rm -f *.xcodeproj/*.mode1v3
	rm -f *.xcodeproj/*.pbxuser

reload:
	@launchctl unload /Library/LaunchAgents/org.pqrs.KeyRemap4MacBook.server.plist
	sudo ./scripts/unload.sh
	sudo ./scripts/load.sh
	@launchctl load /Library/LaunchAgents/org.pqrs.KeyRemap4MacBook.server.plist
	@./scripts/wait.sh

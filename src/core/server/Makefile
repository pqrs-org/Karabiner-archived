#CONFIGURATION = Debug
CONFIGURATION = Release

all:
	$(MAKE) -C ../../../pkginfo
	$(MAKE) -C ../bridge
	xcodebuild -alltargets -configuration $(CONFIGURATION) build

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

run:
	-launchctl unload /Library/LaunchAgents/org.pqrs.KeyRemap4MacBook.server.plist 2>/dev/null
	./build/Release/KeyRemap4MacBook_server.app/Contents/MacOS/KeyRemap4MacBook_server

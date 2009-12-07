#CONFIGURATION = Debug
CONFIGURATION = Release

all:
	xcodebuild -alltargets -configuration $(CONFIGURATION) build

clean:
	rm -rf build
	rm -f *.xcodeproj/*.mode1
	rm -f *.xcodeproj/*.mode1v3
	rm -f *.xcodeproj/*.pbxuser

run:
	rm -f /tmp/KeyRemap4MacBook_server
	./build/Release/KeyRemap4MacBook_server.app/Contents/MacOS/KeyRemap4MacBook_server /tmp

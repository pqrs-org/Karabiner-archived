#CONFIGURATION = Debug
CONFIGURATION = Release

all:
	xcodebuild -alltargets -configuration $(CONFIGURATION) build

clean:
	rm -rf build
	rm -f *.xcodeproj/*.mode1
	rm -f *.xcodeproj/*.mode1v3
	rm -f *.xcodeproj/*.pbxuser

run: all
	./build/Release/KeyRemap4MacBook_multitouchextension.app/Contents/MacOS/KeyRemap4MacBook_multitouchextension

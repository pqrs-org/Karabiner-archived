#CONFIGURATION = Debug
CONFIGURATION = Release

all:
	./setversion.sh
	xcodebuild -alltargets -configuration $(CONFIGURATION) build

clean:
	xcodebuild -alltargets -configuration $(CONFIGURATION) clean
	rm -rf build
	rm -f *.xcodeproj/*.mode1
	rm -f *.xcodeproj/*.mode1v3
	rm -f *.xcodeproj/*.pbxuser

install:
	sudo rsync -a build/Release/KeyRemap4MacBook.prefPane /Library/PreferencePanes/

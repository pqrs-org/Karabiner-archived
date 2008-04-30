#CONFIGURATION = Debug
CONFIGURATION = Release

all:
	xcodebuild -alltargets -configuration $(CONFIGURATION) build

clean:
	xcodebuild -alltargets -configuration $(CONFIGURATION) clean
	rm -rf build
	rm -f *.xcodeproj/*.mode1
	rm -f *.xcodeproj/*.mode1v3
	rm -f *.xcodeproj/*.pbxuser

install:
	rsync -a --delete build/Release/KeyRemap4MacBook.prefPane $(HOME)/Library/PreferencePanes/

uninstall:
	rm -rf $(HOME)/Library/PreferencePanes/KeyRemap4MacBook.prefPane

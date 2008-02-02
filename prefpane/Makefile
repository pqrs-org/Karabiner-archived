#CONFIGURATION = Debug
CONFIGURATION = Release

all:
	xcodebuild -alltargets -configuration $(CONFIGURATION) build

clean:
	xcodebuild -alltargets -configuration $(CONFIGURATION) clean
	rm -rf build

install:
	sudo rsync -a build/Release/KeyRemap4MacBook.prefPane /Library/PreferencePanes/

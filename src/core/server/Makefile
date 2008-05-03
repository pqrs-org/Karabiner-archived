#CONFIGURATION = Debug
CONFIGURATION = Release

all:
	xcodebuild -alltargets -configuration $(CONFIGURATION) build

clean:
	xcodebuild -alltargets -configuration $(CONFIGURATION) clean
	rm -rf build

run:
	sudo ./build/Release/KeyRemap4MacBook_server

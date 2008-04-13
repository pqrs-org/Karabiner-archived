#CONFIGURATION = Debug
CONFIGURATION = Release

all:
	xcodebuild -alltargets -configuration $(CONFIGURATION) build

clean:
	xcodebuild -alltargets -configuration $(CONFIGURATION) clean
	rm -rf build

run:
	./build/Release/KeyRemap4MacBook-server.app/Contents/MacOS/KeyRemap4MacBook-server

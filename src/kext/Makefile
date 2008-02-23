#CONFIGURATION = Debug
CONFIGURATION = Release

all:
	$(MAKE) -C config
	xcodebuild -alltargets -configuration $(CONFIGURATION) build

clean:
	xcodebuild -alltargets -configuration $(CONFIGURATION) clean
	rm -rf build

reload:
	sudo ./scripts/unload.sh
	sudo ./scripts/load.sh

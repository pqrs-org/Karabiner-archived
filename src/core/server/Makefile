#CONFIGURATION = Debug
CONFIGURATION = Release

AUTOUPDATE_ICON = 'build/Release/Karabiner.app/Contents/Frameworks/Sparkle.framework/Versions/A/Resources/Autoupdate.app/Contents/Resources/AppIcon.icns'

all:
	$(MAKE) -C Resources
	$(MAKE) -C ../../../pkginfo
	$(MAKE) -C ../../bridge/generator/bridge_version
	$(MAKE) -C ../../bridge/generator/config
	$(MAKE) -C ../../lib/xml_compiler
	(cd Pods && xcodebuild -configuration $(CONFIGURATION) SYMROOT=$(CURDIR)/build)
	xcodebuild -alltargets -configuration $(CONFIGURATION) build
	[ -f $(AUTOUPDATE_ICON) ] || exit 1
	install -m 644 ../../share/Resources/app.icns $(AUTOUPDATE_ICON)

buildonly:
	(cd Pods && xcodebuild -configuration $(CONFIGURATION) SYMROOT=$(CURDIR)/build)
	xcodebuild -alltargets -configuration $(CONFIGURATION) build

clean:
	rm -rf build
	rm -f Info.plist
	rm -f *.xcodeproj/*.mode1
	rm -f *.xcodeproj/*.mode1v3
	rm -f *.xcodeproj/*.pbxuser
	rm -rf *.xcodeproj/xcuserdata
	rm -rf *.xcodeproj/project.xcworkspace/xcuserdata
	$(MAKE) -C ../../lib/xml_compiler clean

xcode:
	open *.xcworkspace

run: all
	open ./build/Release/Karabiner.app

relaunch:
	/Applications/Karabiner.app/Contents/Library/bin/karabiner relaunch

podupdate:
	pod update
	pod install --no-repo-update

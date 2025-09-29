PROJECT_NAME = track38
APP_ID = io.github.fidoriel.track38
ARCH := $(shell uname -m)
AppImageRelease := appimagetool-$(ARCH).AppImage
AppImageURL := https://github.com/AppImage/appimagetool/releases/download/continuous

run: build/track38-$(ARCH).AppImage
	./build/track38-$(ARCH).AppImage
.PHONY: run

src/wxWidgets/configure:
	# Just a file from a submodule
	git submodule update --init

src/wxWidgets/3rdparty/pcre/pcre2-config.in: src/wxWidgets/configure
	# Just a file from a "recursive" submodule
	git submodule update --init --recursive

build/: src/wxWidgets/configure src/wxWidgets/3rdparty/pcre/pcre2-config.in
	cmake -B build

build/track38: build/
	(cd build; make -j)

build/track38-$(ARCH).AppImage: build/track38
	install -Dd "build/AppDir/usr/share/$(PROJECT_NAME)"
	cp -r "resources/tracks" "build/AppDir/usr/share/$(PROJECT_NAME)/"
	cp -r "resources/icons" "build/AppDir/usr/share/$(PROJECT_NAME)/"
	# metainfo
	install -Dm644 "resources/metainfo/$(PROJECT_NAME).appdata.xml" "build/AppDir/usr/share/metainfo/$(APP_ID).appdata.xml"
	# Icon
	install -m644 "icons/AppIcon.png" "build/AppDir/usr/share/icons/hicolor/256x256/apps/$(PROJECT_NAME).png"
	# .desktop file
	install -Dm755 $(PROJECT_NAME).desktop build/AppDir/usr/share/applications/$(APP_ID).desktop

	(cd build/; wget -N "$(AppImageURL)/$(AppImageRelease)")
	chmod a+x build/$(AppImageRelease)

	(cd build/AppDir/; ln -sf usr/share/applications/$(APP_ID).desktop)
	(cd build/AppDir/; ln -sf usr/share/icons/hicolor/256x256/apps/$(PROJECT_NAME).png)
	(cd build/; wget -N https://raw.githubusercontent.com/AppImage/AppImageKit/master/src/AppRun.c)
	gcc build/AppRun.c -o build/AppDir/AppRun
	install -D build/$(PROJECT_NAME) build/AppDir/usr/bin/$(PROJECT_NAME)
	(cd build/; ./$(AppImageRelease) AppDir)


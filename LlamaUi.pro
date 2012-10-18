TEMPLATE = subdirs
SUBDIRS += src

OTHER_FILES += qml/main.qml \
	qml/ListViewHighlight.qml \
	qml/KeyboardDelegate.qml \
	qml/GameView.qml \
	qml/GameDelegate.qml \
	qml/GridViewHighlight.qml \
	qml/EmulatorView.qml \
	qml/EmulatorDelegate.qml \
	qml/SearchView.qml \
	qml/SystemView.qml \
	qml/SystemDelegate.qml

PREFIX=/usr/local/llamaui

qml.path = $$PREFIX/share/qml
qml.files = $$OTHER_FILES

sounds.path = $$PREFIX/share/sounds
sounds.files = sounds/move.wav \
	sounds/select.wav \
	sounds/start.wav \
	sounds/swish.wav \
	sounds/toggle_on.wav \
	sounds/toggle_off.wav

images.path = $$PREFIX/share/images
images.files = images/arrow-down-disabled.png \
	images/arrow-down.png \
	images/arrow-left-disabled.png \
	images/arrow-left.png \
	images/arrow-right-disabled.png \
	images/arrow-right.png \
	images/arrow-up-disabled.png \
	images/arrow-up.png \
	images/favourite.png \
	images/checked.png

wallpaper.path = $$PREFIX/share/wallpapers
wallpaper.files = wallpapers/glossymortalkombat.jpg

var.path = $$PREFIX/var
var.files = var/llamaui.sqlite

config.path = $$PREFIX/etc
config.files = llamaui.conf.example

app.path = $$PREFIX/bin
app.files = src/llamaui

INSTALLS += qml sounds images wallpaper var config app

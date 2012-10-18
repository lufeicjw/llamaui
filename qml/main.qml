import QtQuick 1.1
import net.randalflagg.llamaui 1.0

Rectangle {
	id: window
	width: 1024
	height: 768
	color: "black"

	function emulatorChanged() {
		gameView.setCurrentEmulator( emulatorView.selectedEmulator );
		searchView.setSearchText( "" );
	}

	function showEmulatorView() {
		soundEngine.playSound( SoundEngine.SwishSound );
		emulatorView.state = "shown";
	}

	function showSearchView() {
		soundEngine.playSound( SoundEngine.SwishSound );
		searchView.state = "shown";
	}

	function showSystemView() {
		soundEngine.playSound( SoundEngine.SwishSound );
		systemView.state = "shown";
	}

	function hideEmulatorView() {
		soundEngine.playSound( SoundEngine.SwishSound );
		emulatorView.state = "hidden";
	}

	function hideSearchView() {
		soundEngine.playSound( SoundEngine.SwishSound );
		searchView.state = "hidden";
	}

	function hideSystemView() {
		soundEngine.playSound( SoundEngine.SwishSound );
		systemView.state = "hidden";
	}

	function changeWallpaper() {
		if ( !config.hasValue( "ui", "wallpapers" ) )
			return "";

		var wallpapers = config.values( "ui", "wallpapers" );
		wallpaper.source = wallpapers[ Math.round( Math.random() * ( wallpapers.length - 1 ) ) ];
	}

	// Background wallpaper
	Image {
		id: wallpaper
		width: parent.width
		height: parent.height
		fillMode: Image.PreserveAspectCrop
		smooth: true
		source: ""
	}

	// Wallpaper rotate timer
	Timer {
		id: wallpaperTimer
		repeat: true
		triggeredOnStart: true
		onTriggered: changeWallpaper();
	}

	GameView {
		id: gameView
		x: 0
		y: 0
		width: window.width
		height: window.height
	}

	EmulatorView {
		id: emulatorView
		x: 0
		y: 0
		width: window.width / 3
		height: window.height
		state: "hidden"

		states: [
			State {
				name: "shown"

				PropertyChanges {
					target: gameView
					x: window.width / 3
					width: window.width * 2 / 3
					focus: false
				}
				PropertyChanges {
					target: emulatorView
					opacity: 1
					focus: true
				}
			},
			State {
				name: "hidden"

				PropertyChanges {
					target: gameView
					x: 0
					width: window.width
					focus: true
				}
				PropertyChanges {
					target: emulatorView
					opacity: 0
					focus: false
				}
			}
		]

		transitions: [
			Transition {
				from: "hidden"
				to: "shown"

				NumberAnimation {
					properties: "x,width"
					duration: 300
					easing.type: Easing.OutQuad
				}
				NumberAnimation {
					properties: "opacity"
					duration: 300
					easing.type: Easing.InQuad
				}
			},
			Transition {
				from: "shown"
				to: "hidden"

				NumberAnimation {
					properties: "opacity"
					duration: 300
					easing.type: Easing.OutQuad
				}
				NumberAnimation {
					properties: "x,width"
					duration: 300
					easing.type: Easing.InQuad
				}
			}
		]
	}

	SearchView {
		id: searchView
		x: 0
		y: 0
		width: 640
		height: window.height
		state: "hidden"

		states: [
			State {
				name: "shown"

				PropertyChanges {
					target: gameView
					x: 640
					width: window.width - 640
					focus: false
				}
				PropertyChanges {
					target: searchView
					opacity: 1
					focus: true
				}
			},
			State {
				name: "hidden"

				PropertyChanges {
					target: gameView
					width: window.width
					focus: true
				}
				PropertyChanges {
					target: searchView
					opacity: 0
					focus: false
				}
			}
		]

		transitions: [
			Transition {
				from: "hidden"
				to: "shown"

				NumberAnimation {
					properties: "x,width"
					duration: 300
					easing.type: Easing.OutQuad
				}
				NumberAnimation {
					properties: "opacity"
					duration: 300
					easing.type: Easing.InQuad
				}
			},
			Transition {
				from: "shown"
				to: "hidden"

				NumberAnimation {
					properties: "opacity"
					duration: 300
					easing.type: Easing.OutQuad
				}
				NumberAnimation {
					properties: "x,width"
					duration: 300
					easing.type: Easing.InQuad
				}
			}
		]
	}

	SystemView {
		id: systemView
		x: 0
		y: 0
		width: window.width / 3
		height: window.height
		state: "hidden"

		states: [
			State {
				name: "shown"

				PropertyChanges {
					target: gameView
					x: window.width / 3
					width: window.width * 2 / 3
					focus: false
				}
				PropertyChanges {
					target: systemView
					opacity: 1
					focus: true
				}
			},
			State {
				name: "hidden"

				PropertyChanges {
					target: gameView
					width: window.width
					focus: true
				}
				PropertyChanges {
					target: systemView
					opacity: 0
					focus: false
				}
			}
		]

		transitions: [
			Transition {
				from: "hidden"
				to: "shown"

				NumberAnimation {
					properties: "x,width"
					duration: 300
					easing.type: Easing.OutQuad
				}
				NumberAnimation {
					properties: "opacity"
					duration: 300
					easing.type: Easing.InQuad
				}
			},
			Transition {
				from: "shown"
				to: "hidden"

				NumberAnimation {
					properties: "opacity"
					duration: 300
					easing.type: Easing.OutQuad
				}
				NumberAnimation {
					properties: "x,width"
					duration: 300
					easing.type: Easing.InQuad
				}
			}
		]
	}

	Component.onCompleted: {
		searchView.search.connect( gameView.search );
		emulatorView.selectedEmulatorChanged.connect( emulatorChanged );
		emulatorView.itemToggled.connect( gameRomModel.setFilter );
		gameView.launchGame.connect( gameProcess.startGame );

		gameView.showEmulatorView.connect( showEmulatorView );
		gameView.showSearchView.connect( showSearchView );
		gameView.showSystemView.connect( showSystemView );

		emulatorView.hideView.connect( hideEmulatorView );
		searchView.hideView.connect( hideSearchView );
		systemView.hideView.connect( hideSystemView );

		// Start the wallpaper rotation if configured
		if ( config.hasValue( "ui", "wallpaperRotateInterval" ) ) {
			wallpaperTimer.interval = config.value( "ui", "wallpaperRotateInterval" ) * 1000;
			wallpaperTimer.start();
		} else {
			// Otherwise just show a random wallpaper and call it good
			changeWallpaper();
		}
	}
}

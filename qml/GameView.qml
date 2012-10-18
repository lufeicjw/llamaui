import QtQuick 1.1
import net.randalflagg.llamaui 1.0

Item {
	id: gameView
	focus: true

	property string currentEmulator: ""
	property string currentRom: ""

	signal launchGame( string emulator, string rom )
	signal showEmulatorView();
	signal showSearchView();
	signal showSystemView();

	function setCurrentEmulator( emulator ) {
		currentEmulator = emulator
	}

	function search( searchText ) {
		currentRom = "";
		gameRomModel.setSearchText( searchText );
	}

	function gameListReset() {
		var index = listView.model.indexOfRom( currentEmulator, currentRom );

		if ( index !== -1 || listView.model.rowCount === 0 )
			listView.currentIndex = index;
		else
			listView.currentIndex = 0;

		updateTitleText();
	}

	function updateTitleText() {
		var text = "";
		var searchText = listView.model.searchText;
		var isFilteredByRecentlyPlayed = listView.model.isFilterSet( "mostrecent" );
		var isFilteredByMostPlayed = listView.model.isFilterSet( "mostplayed" );
		var isFilteredByFavourites = listView.model.isFilterSet( "favourites" );
		var isFilteredByNeverPlayed = listView.model.isFilterSet( "neverplayed" );

		var emulatorIndex = emulatorModel.indexOfEmulator( currentEmulator );

		if ( emulatorIndex !== -1 )
			text = emulatorModel.descriptionAtIndex( emulatorIndex );
		else
			text = "Unknown";

		if ( searchText !== "" || isFilteredByRecentlyPlayed || isFilteredByMostPlayed || isFilteredByFavourites ) {
			text += " filtered by ";

			var parts = [];

			if ( searchText !== "" )
				parts.push( '"' + searchText + '"' );

			if ( isFilteredByRecentlyPlayed )
				parts.push( "recently played" );

			if ( isFilteredByMostPlayed )
				parts.push( "most played" );

			if ( isFilteredByFavourites )
				parts.push( "favourites" );

			if ( isFilteredByNeverPlayed )
				parts.push( "never played" );

			for ( var i = 0; i < parts.length; ++i ) {
				text += parts[ i ];

				if ( i < parts.length - 2 )
					text += ", ";
				else if ( i < parts.length - 1 )
					text += " and ";
			}
		}

		var gameCount = gameRomModel.count();
		text += " (" + gameCount + " game" + ( gameCount === 1 ? "" : "s" ) + ")";
		titleText.text = text;
	}

	function processKeyAction( keys ) {
		var action = keyMap.actionForKeys( keys );

		if ( action === KeyboardMap.UpAction ) {
			soundEngine.playSound( SoundEngine.MoveSound );
			listView.decrementCurrentIndex();
		} else if ( action === KeyboardMap.DownAction ) {
			soundEngine.playSound( SoundEngine.MoveSound );
			listView.incrementCurrentIndex();
		} else if ( action === KeyboardMap.SelectAction ) {
			soundEngine.playSound( SoundEngine.StartSound );
			gameView.launchGame( listView.currentItem.emulator, listView.currentItem.rom );
		} else if ( action === KeyboardMap.LeftAction ) {
			switchViewLeft();
		} else if ( action === KeyboardMap.RightAction ) {
			switchViewRight();
		}
	}

	Text {
		id: titleText
		text: ""
		anchors {
			horizontalCenter: parent.horizontalCenter
			top: parent.top
		}
		color: "white"
		style: Text.Outline
		styleColor: "black"
		font {
			pointSize: 18
			family: "Mortal Kombat 3"
			bold: true
		}
	}

	Image {
		id: upArrow
		anchors {
			horizontalCenter: parent.horizontalCenter
			top: titleText.bottom
		}
		source: "../images/arrow-up-disabled.png"
	}

	ListView {
		id: listView
		clip: true
		anchors {
			top: upArrow.bottom
			bottom: downArrow.top
		}
		width: gameView.width
		model: gameRomModel
		delegate: GameDelegate { }
		highlight: ListViewHighlight { }
		highlightFollowsCurrentItem: false
		keyNavigationWraps: true
		spacing: 5

		onCurrentIndexChanged: {
			if ( currentIndex == 0 )
				upArrow.source =  "../images/arrow-up-disabled.png"
			else
				upArrow.source = "../images/arrow-up.png"

			if ( currentIndex == count - 1 )
				downArrow.source = "../images/arrow-down-disabled.png"
			else
				downArrow.source = "../images/arrow-down.png"
		}
	}

	Image {
		id: downArrow
		anchors {
			horizontalCenter: parent.horizontalCenter
			bottom: parent.bottom
		}
		source: "../images/arrow-down-disabled.png"
	}

	Component.onCompleted: {
		gameRomModel.modelReset.connect( gameListReset );
		currentEmulator = "mame";
	}

	Keys.onPressed: {
		keyMap.addKeyToQueue( event.key );
	}

	Keys.onReleased: {
		var action = keyMap.queueToActionAndClear();

		if ( action === KeyboardMap.NoAction )
			return;

		if ( action === KeyboardMap.UpAction ) {
			soundEngine.playSound( SoundEngine.MoveSound );
			listView.decrementCurrentIndex();
		} else if ( action === KeyboardMap.DownAction ) {
			soundEngine.playSound( SoundEngine.MoveSound );
			listView.incrementCurrentIndex();
		} else if ( action === KeyboardMap.SelectAction ) {
			soundEngine.playSound( SoundEngine.StartSound );
			gameView.launchGame( listView.currentItem.emulator, listView.currentItem.rom );
		} else if ( action === KeyboardMap.EmulatorMenuAction ) {
			showEmulatorView();
		} else if ( action === KeyboardMap.SearchMenuAction ) {
			showSearchView();
		} else if ( action === KeyboardMap.SystemMenuAction ) {
			showSystemView();
		} else if ( action === KeyboardMap.ToggleFavouriteAction ) {
			listView.model.toggleFavourite( listView.currentItem.emulator, listView.currentItem.rom );
		}
	}

	onCurrentEmulatorChanged: {
		currentRom = gameProcess.lastGamePlayed( currentEmulator );
		listView.model.setEmulator( currentEmulator );
	}
}

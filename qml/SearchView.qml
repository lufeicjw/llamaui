import QtQuick 1.1
import net.randalflagg.llamaui 1.0

Item {
	id: searchView
	width: 100 * 5
	height: parent.height
	focus: true

	property string text
	property string matchedText

	signal search( string searchText )
	signal hideView()

	function initKeyboardModel() {
		var visualKeys = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.+-/␠␡";
		var keyCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.+-/ \x08";

		for ( var i = 0; i < visualKeys.length; ++i ) {
			var data = { "character": visualKeys.charAt( i ), "key": keyCharacters.charAt( i ) };
			keyboardModel.append( data );
		}
	}

	function processCurrentCharacter() {
		var character = keyboardModel.get( listView.currentIndex ).key;

		if ( character.charCodeAt( 0 ) === 0x08 ) {
			if ( matchedText != "" )
				matchedText = "";
			else
				text = text.substring( 0, text.length - 1 );
		} else {
			text += character;
			matchedText = gameRomModel.retrieveSearchText( text );
		}

		updateTextLabel();
	}

	function updateTextLabel() {
		if ( matchedText != "" ) {
			textLabel.text = "<font color=\"#ffffff\">" + text + "</font><font color=\"#7f7f7f\">" + matchedText.substring( text.length ) + "</font>";
			searchView.search( matchedText );
		} else {
			textLabel.text = "<font color=\"#ffffff\">" + text + "</font>";
			searchView.search( text );
		}
	}

	function setSearchText( searchText ) {
		text = searchText;
		matchedText = searchText;
		updateTextLabel();
	}

	Text {
		id: title
		text: "Game search"
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

	Item {
		id: searchText
		width: parent.width - 10
		height: textLabel.height * 1.5
		anchors {
			top: title.bottom
			topMargin: 20
		}

		Rectangle {
			id: border
			color: "#00000000"
			radius: 10
			border.color: "#7f7f7f"
			width: parent.width
			height: parent.height - 1
		}

		Text {
			id: textLabel
			text: ""
			style: Text.Outline
			styleColor: "black"
			z: 1
			anchors.centerIn: parent
			font {
				pointSize: 20
				family: "Mortal Kombat 3"
				bold: true
			}
			color: "white"
		}
	}

	GridView {
		id: listView
		clip: true
		width: parent.width
		height: parent.height - title.height - searchText.height
		cellWidth: 100
		cellHeight: 80
		keyNavigationWraps: true
		anchors {
			top: searchText.bottom
			topMargin: 20
		}
		model: keyboardModel
		delegate: KeyboardDelegate { }
		highlight: GridViewHighlight { }
		highlightFollowsCurrentItem: true

		ListModel {
			id: keyboardModel
		}
	}

	Component.onCompleted: {
		initKeyboardModel();
	}

	Keys.onPressed: {
		keyMap.addKeyToQueue( event.key );
	}

	Keys.onReleased: {
		var action = keyMap.queueToActionAndClear();
//		var columns = Math.floor( listView.width / 100 );
//		var column = listView.currentIndex % columns;

		if ( action === KeyboardMap.UpAction ) {
			soundEngine.playSound( SoundEngine.MoveSound );
			listView.moveCurrentIndexUp();
		} else if ( action === KeyboardMap.DownAction ) {
			soundEngine.playSound( SoundEngine.MoveSound );
			listView.moveCurrentIndexDown();
		} else if ( action === KeyboardMap.RightAction ) {
			soundEngine.playSound( SoundEngine.MoveSound );
			listView.moveCurrentIndexRight();
		} else if ( action === KeyboardMap.LeftAction ) {
			soundEngine.playSound( SoundEngine.MoveSound );
			listView.moveCurrentIndexLeft();
		} else if ( action === KeyboardMap.SelectAction ) {
			soundEngine.playSound( SoundEngine.SelectSound );
			processCurrentCharacter();
		} else if ( action === KeyboardMap.SearchMenuAction ) {
			gameRomModel.storeSearchText();
			hideView();
		}
	}
}

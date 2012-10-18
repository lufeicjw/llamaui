import QtQuick 1.1
import net.randalflagg.llamaui 1.0

Item {
	id: systemView
	focus: true

	signal hideView();

	Text {
		id: title
		text: "System menu"
		style: Text.Outline
		styleColor: "black"
		horizontalAlignment: Text.AlignHCenter
		font {
			family: "Mortal Kombat 3"
			pointSize: 18
			bold: true
		}
		color: "white"
		anchors {
			horizontalCenter: parent.horizontalCenter
			top: parent.top
		}
	}

	Image {
		id: upArrow
		anchors {
			top: title.bottom
			horizontalCenter: parent.horizontalCenter
		}
		source: "../images/arrow-up-disabled.png"
	}

	ListView {
		id: listView

		clip: true
		width: parent.width
		anchors {
			top: upArrow.bottom
			bottom: downArrow.top
		}
		model: systemModel
		delegate: SystemDelegate { }
		highlight: ListViewHighlight {
			highlightColour: "#7fff0000"
		}
		highlightFollowsCurrentItem: true
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
			bottom: parent.bottom
			horizontalCenter: parent.horizontalCenter
		}
		source: "../images/arrow-down-disabled.png"
	}

	Keys.onPressed: {
		keyMap.addKeyToQueue( event.key );
	}

	Keys.onReleased: {
		var action = keyMap.queueToActionAndClear();

		if ( action === KeyboardMap.UpAction ) {
			soundEngine.playSound( SoundEngine.MoveSound );
			listView.decrementCurrentIndex();
		} else if ( action === KeyboardMap.DownAction ) {
			soundEngine.playSound( SoundEngine.MoveSound );
			listView.incrementCurrentIndex();
		} else if ( action === KeyboardMap.SelectAction ) {
			soundEngine.playSound( SoundEngine.SelectSound );
			systemProcess.executeCommand( listView.model.commandAtIndex( listView.currentIndex ) );
		} else if ( action === KeyboardMap.SystemMenuAction ) {
			hideView();
		}
	}
}

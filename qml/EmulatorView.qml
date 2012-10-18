import QtQuick 1.1
import net.randalflagg.llamaui 1.0

Item {
	id: emulatorView
	focus: true

	signal hideView();
	signal itemToggled( string item, bool checked );

	property string selectedEmulator: "mame"

	Image {
		id: upArrow
		anchors {
			top: parent.top
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
		model: emulatorModel
		delegate: EmulatorDelegate { }
		highlight: ListViewHighlight { }
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

	Component.onCompleted: {
		var index = listView.model.indexOfEmulator( selectedEmulator );

		if ( index !== -1 )
			listView.currentIndex = index;
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
			if ( listView.currentItem.itemIsCheckable ) {
				soundEngine.playSound( listView.currentItem.itemIsChecked ? SoundEngine.ToggleOffSound : SoundEngine.ToggleOnSound );
				listView.model.toggleItemAtIndex( listView.currentIndex );
				itemToggled( listView.currentItem.itemName, listView.currentItem.itemIsChecked );
			} else {
				soundEngine.playSound( SoundEngine.SelectSound );
				selectedEmulator = listView.currentItem.itemName;
			}
		} else if ( action === KeyboardMap.EmulatorMenuAction ) {
			hideView();
		}
	}
}

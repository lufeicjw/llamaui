import QtQuick 1.1

Item {
	id: gameDelegate
	height: text.height * 1.5
	width: parent.width

	property string emulator: model.emulator
	property string rom: model.rom

	Column {
		id: text
		anchors {
			verticalCenter: parent.verticalCenter
			horizontalCenter: parent.horizontalCenter
		}

		Text {
			id: title
			anchors.horizontalCenter: parent.horizontalCenter
			text: name
			style: Text.Outline
			styleColor: "black"
			elide: Text.ElideRight
			font {
				pointSize: 20
				family: "Mortal Kombat 3"
				bold: true
			}
			color: "white"

			Behavior on color {
				PropertyAnimation { }
			}
		}

		Text {
			id: playDetails
			anchors.horizontalCenter: parent.horizontalCenter
			text: details
			style: Text.Outline
			styleColor: "black"
			font {
				pointSize: 12
				family: "Mortal Kombat 3"
			}
			color: "white"
		}
	}

	Image {
		id: favouriteIcon
		source: "../images/favourite.png"
		anchors {
			verticalCenter: parent.verticalCenter
			left: parent.left
			leftMargin: 30
		}
		visible: favourite
	}

	SequentialAnimation {
		id: pulseAnimation
		running: false
		loops: Animation.Infinite

		ColorAnimation {
			target: title;
			property: "color";
			to: "black";
			duration: 500
			easing.type: Easing.InQuad
		}
		ColorAnimation {
			target: title;
			property: "color";
			to: "red";
			duration: 500
			easing.type: Easing.OutQuad
		}
	}

	states: [
		State {
			name: "selected"
			when: gameDelegate.ListView.isCurrentItem && model.lastPlayReturnCode === 0

			PropertyChanges {
				target: pulseAnimation
				running: true
			}
		},
		State {
			name: "error"
			when: model.lastPlayReturnCode !== 0

			PropertyChanges {
				target: title
				color: "grey"
			}
		},
		State {
			name: ""

			PropertyChanges {
				target: title
				color: "white"
			}
		}
	]
}

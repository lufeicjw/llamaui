import QtQuick 1.1

Item {
	id: keyboardDelegate
	width: 100
	height: 80

	Text {
		id: text
		anchors {
			verticalCenter: parent.verticalCenter
			horizontalCenter: parent.horizontalCenter
		}
		text: character
		style: Text.Outline
		styleColor: "black"
		font {
			pointSize: 20
			bold: true
			family: "Mortal Kombat 3"
		}
		color: "white"

		Behavior on color {
			PropertyAnimation { }
		}
	}

	SequentialAnimation {
		id: pulseAnimation
		running: false
		loops: Animation.Infinite

		ColorAnimation {
			target: text;
			property: "color";
			to: "black";
			duration: 500
			easing.type: Easing.InQuad
		}
		ColorAnimation {
			target: text;
			property: "color";
			to: "red";
			duration: 500
			easing.type: Easing.OutQuad
		}
	}

	states: [
		State {
			name: "selected"
			when: keyboardDelegate.GridView.isCurrentItem

			PropertyChanges {
				target: pulseAnimation
				running: true
			}
		},
		State {
			name: ""

			PropertyChanges {
				target: text
				color: "white"
			}
		}
	]
}

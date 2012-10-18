import QtQuick 1.1

Item {
	id: systemDelegate
	height: text.height * 1.5
	width: parent.width

	Text {
		id: text
		anchors {
			verticalCenter: parent.verticalCenter
			horizontalCenter: parent.horizontalCenter
		}
		text: name
		style: Text.Outline
		styleColor: "black"
		font {
			family: "Mortal Kombat 3"
			pointSize: 20
			bold: true
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
			to: "white";
			duration: 500
			easing.type: Easing.InQuad
		}
		ColorAnimation {
			target: text;
			property: "color";
			to: "darkred";
			duration: 500
			easing.type: Easing.OutQuad
		}
	}

	states: [
		State {
			name: "selected"
			when: systemDelegate.ListView.isCurrentItem

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

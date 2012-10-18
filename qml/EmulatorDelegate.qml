import QtQuick 1.1

Item {
	id: emulatorDelegate
	height: text.height * 1.5
	width: parent.width

	property string itemName: name
	property string itemDescription: description
	property bool itemIsCheckable: isCheckable
	property bool itemIsChecked: isChecked

	Text {
		id: text
		anchors {
			verticalCenter: parent.verticalCenter
			horizontalCenter: parent.horizontalCenter
		}
		text: description
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

	Image {
		id: checkedIcon
		anchors {
			verticalCenter: parent.verticalCenter
			left: parent.left
			leftMargin: 20
		}
		source: "../images/checked.png"
		visible: isChecked
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
			when: emulatorDelegate.ListView.isCurrentItem

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

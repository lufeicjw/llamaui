import QtQuick 1.1

Rectangle {
	x: listView.currentItem ? listView.currentItem.x + 5 : 0
	y: listView.currentItem ? listView.currentItem.y : 0
	width: listView.currentItem ? listView.currentItem.width - 10 : 0
	height: listView.currentItem ? listView.currentItem.height : 0
	radius: 10
	border.color: Qt.darker( highlightColour, 1.3 )
	gradient: Gradient {
		GradientStop {
			position: 0.0
			color: highlightColour
		}
		GradientStop {
			position: 0.5
			color: Qt.darker( highlightColour, 4.0 )
		}
		GradientStop {
			position: 0.51
			color: Qt.darker( highlightColour, 100.0 )
		}
		GradientStop {
			position: 1.0
			color: Qt.darker( highlightColour, 3.0 )
		}
	}

	property color highlightColour: "#7fafafaf"

	Behavior on y {
		PropertyAnimation { }
	}

	Behavior on x {
		PropertyAnimation { }
	}
}

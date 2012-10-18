import QtQuick 1.1

Rectangle {
	radius: 10
	border.color: "#7f7f7f"
	y: listView.currentItem.y
	x: listView.currentItem.x
	width: listView.currentItem.width - 10
	height: listView.currentItem.height
	gradient: Gradient {
		GradientStop {
			position: 0.0
			color: "#7fafafaf"
		}
		GradientStop {
			position: 0.5
			color: "#7f2f2f2f"
		}
		GradientStop {
			position: 0.51
			color: "#7f000000"
		}
		GradientStop {
			position: 1.0
			color: "#7f3f3f3f"
		}
	}

	Behavior on y {
		PropertyAnimation { }
	}

	Behavior on x {
		PropertyAnimation { }
	}
}

import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.2

Item
{
	property var pinOutputSlider1: fugio.output( "Slider1" )
	property var pinOutputSlider2: fugio.output( "Slider2" )
	property var pinOutputSlider3: fugio.output( "Slider3" )
	property var pinOutputSlider4: fugio.output( "Slider4" )

	Grid {
		columns: 4
		columnSpacing: 32
		horizontalItemAlignment: Grid.AlignHCenter

		Slider {
			id: slider1
			orientation: Qt.Vertical

			Component.onCompleted: {
				value = pinOutputSlider1.control.value()
			}

			onValueChanged: {
				pinOutputSlider1.control.setValue( value )

				fugio.pinUpdated( pinOutputSlider1 )
 			}
		}

		Slider {
			id: slider2
			orientation: Qt.Vertical

			Component.onCompleted: {
				value = pinOutputSlider2.control.value()
			}

			onValueChanged: {
				pinOutputSlider2.control.setValue( value )

				fugio.pinUpdated( pinOutputSlider2 )
 			}
		}

		Slider {
			id: slider3
			orientation: Qt.Vertical

			Component.onCompleted: {
				value = pinOutputSlider3.control.value()
			}

			onValueChanged: {
				pinOutputSlider3.control.setValue( value )

				fugio.pinUpdated( pinOutputSlider3 )
 			}
		}

		Slider {
			id: slider4
			orientation: Qt.Vertical

			Component.onCompleted: {
				value = pinOutputSlider4.control.value()
			}

			onValueChanged: {
				pinOutputSlider4.control.setValue( value )

				fugio.pinUpdated( pinOutputSlider4 )
 			}
		}

		Label {
			text: pinOutputSlider1.name
		}

		Label {
			text: "Slider2"
		}

		Label {
			text: "Slider3"
		}

		Label {
			text: "Slider4"
		}
	}

}

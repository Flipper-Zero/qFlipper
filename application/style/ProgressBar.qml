import QtQuick 2.15
import QtQuick.Templates 2.15 as T
import QtQuick.Controls 2.15

import Theme 1.0

T.ProgressBar {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    contentItem: Item {
        id: content
        anchors.fill: parent
        anchors.margins: bg.border.width

        Text {
            id: brightText
            visible: !control.indeterminate
            color: Theme.color.orange
            text:  Math.round(control.value) + "%"
            anchors.centerIn: parent
            font.pixelSize: 48

        }

        Text {
            id: animationText
            visible: control.indeterminate
            color: Theme.color.orange
            anchors.centerIn: parent
            font.pixelSize: 48

            Timer {
                repeat: true
                running: control.indeterminate
                triggeredOnStart: true
                interval: 500

                onTriggered: {
                    if(animationText.text.length === 4) {
                        animationText.text = "."
                    } else {
                        animationText.text += "."
                    }
                }
            }
        }

        Rectangle {
            id: barFill
            clip: true
            visible: !control.indeterminate
            color: Theme.color.orange
            width: visualPosition * parent.width
            height: parent.height

            Text {
                id: darkText

                x: brightText.x
                y: brightText.y

                width: brightText.width
                height: brightText.height

                color: Theme.color.darkorange1
                text: brightText.text
                font: brightText.font
            }
        }
    }

    background: Rectangle {
        id: bg
        anchors.fill: parent
        color: Theme.color.transparent
        border.color: Theme.color.orange
        border.width: 3
        radius: 9
    }
}

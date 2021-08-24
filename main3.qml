import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Column{
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Text {
            width: parent.width
            height:  50
            verticalAlignment: Text.AlignVCenter
            text: ExposedCppInstance.someString
        }

        Item{
            width: parent.width
            height: 50

            Text {
                id:zeitText
                anchors{
                    left: parent.left
                    right: parent.horizontalCenter
                    top: parent.top
                    bottom: parent.bottom
                }
                text: "--"
                verticalAlignment: Text.AlignVCenter
            }
            Button{
                text: "Aktuelle Zeit:"
                anchors{
                    left: parent.horizontalCenter
                    top: parent.top
                    bottom: parent.bottom
                    right: parent.right
                }

                onClicked: zeitText.text = ExposedCppInstance.currentTime()
            }
        }

        Item{
            width: parent.width
            height: 50

            Text {
                id:zaehlerText
                anchors{
                    left: parent.left
                    right: parent.horizontalCenter
                    top: parent.top
                    bottom: parent.bottom
                }
                text: ExposedCppInstance.zaehler
                verticalAlignment: Text.AlignVCenter
            }
            Button{
                text: "Starte Zähler:"
                anchors{
                    left: parent.horizontalCenter
                    top: parent.top
                    bottom: parent.bottom
                    right: parent.right
                }

                onClicked: ExposedCppInstance.starteZaehler()
            }
        }
    }
}

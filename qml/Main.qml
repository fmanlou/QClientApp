import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("QClientApp")

    ListView {
        id: listView
        anchors.fill: parent
        orientation: ListView.Horizontal
        model: client.model
        delegate: Rectangle {
            height: listView.height
            width: listView.width / 2

            Text {
                text: id
                font.pixelSize: 48
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 10
            }

            Text {
                text: value
                font.pixelSize: 120
                anchors.centerIn: parent
            }
        }
    }

    Client {
        id: client
    }
}

import QtQuick
import Cpp

QtObject {
    id: root

    property string discText: "---"
    property Client client: Client {
        ip: "localhost"
        port: 50051
    }
    property var model: client.model
    property Timer autoConnectTimer: Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            if (!client.connected) {
                client.connectToServer();
            }
        }
    }
}

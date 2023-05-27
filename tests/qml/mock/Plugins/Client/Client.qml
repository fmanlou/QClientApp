import QtQuick

QtObject {
    id: clientMock
    property bool connected: false
    property int paramValue: 50
    property int paramIndex: 1
    property int port: 50051
    property string ip: "localhost"
    property bool autoConnect: false
    function connectToServer() {
    }
}

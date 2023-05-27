import QtQuick
import QtTest
import "../../src" as SUT

TestCase {
    name: "ClientTest"

    SUT.Client {
        id: client
    }

    function test_text() {
        verify(client.paramIndexStr == client.discText);
        verify(client.paramValue === client.discText);
        client.client.connected = true;
        verify(client.paramIndexStr == client.client.paramIndex);
        verify(client.paramValue == client.client.paramValue);
        client.client.connected = false;
        verify(client.paramIndexStr == client.discText);
        verify(client.paramValue === client.discText);
    }
}

import QtQuick
import QtTest
import "../../src" as SUT

TestCase {
    name: "ClientTest"

    SUT.Client {
        id: client
    }

    function test_client() {
        verify(client.client.autoConnect);
        verify(client.client.ip === "localhost");
        verify(client.client.port === 50051);
        verify(client.client.connected === true);
    }
}

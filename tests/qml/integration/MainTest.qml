import QtQuick
import QtTest
import "../../src" as SUT

TestCase {
    name: "MainTest"
    when: windowShown

    SUT.Main {
        id: window
    }

    function test_main() {
        wait(4000);
        verify(true);
    }
}

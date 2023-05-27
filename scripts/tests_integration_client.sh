. ./qml/tests/scripts/test_env.sh
$ServerAppExec &
export QML2_IMPORT_PATH=./build/imports/
qmltestrunner -input qml/tests/integration/tst_Client.qml
killall $ServerAppName

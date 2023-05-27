. ./qml/tests/scripts/test_env.sh
$ServerAppExec &
export QML2_IMPORT_PATH=./build/imports/
qmltestrunner -input qml/tests/integration/tst_Main.qml &
sleep 1
killall $ServerAppName
sleep 1
$ServerAppExec &
sleep 1
killall $ServerAppName
sleep 3
exit 0

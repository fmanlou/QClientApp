#include <QGuiApplication>
#include <QQmlApplicationEngine>

auto main(int argc, char *argv[]) -> int {
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

  engine.addImportPath("./imports");
  engine.load("./qml/Main.qml");

  return app.exec();
}
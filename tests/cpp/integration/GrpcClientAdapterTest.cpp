#include <QCoreApplication>
#include <QProcess>
#include <QTimer>
#include <catch2/catch_all.hpp>

#include "GrpcClientAdapter.h"

TEST_CASE("connect", "[GrpcClientAdapter]") {
  QString program =
      "/home/fmanlou/Documents/Github/Monitor/ServerApp/build/bin/ServerApp";
  std::unique_ptr<QProcess> process(new QProcess());
  process->start(program);
  process->waitForStarted();
  QThread::msleep(100);

  int argc = 0;
  QCoreApplication app(argc, nullptr);

  bool connected{false};
  GrpcClientAdapter adapter;

  QTimer::singleShot(0, [&]() {
    adapter.setConnectedNotifier([&](bool b) { connected = b; });
    adapter.connectToServer("localhost:50051");
    QTimer::singleShot(100, [&]() {
      REQUIRE(connected);
      adapter.disconnectFromServer();
      REQUIRE(!connected);
      adapter.connectToServer("localhost:50051");
      QTimer::singleShot(100, [&]() { app.quit(); });
    });
  });

  app.exec();
  process->kill();
  process->waitForFinished();
}

#define CATCH_CONFIG_RUNNER  // 定义此宏以自定义 main 函数
#include <QCoreApplication>
#include <QTimer>
#include <catch2/catch_all.hpp>

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);
  QTimer::singleShot(0, [&] { app.exit(Catch::Session().run(argc, argv)); });
  return app.exec();
}

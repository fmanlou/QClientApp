#include "IClientAdapater.h"

class ClientAdapterMock : public IClientAdapter {
 public:
  void connectToServer(const QString& address) override {
    if (connectToServer_) connectToServer_(address);
  }

  void disconnectFromServer() override {
    if (disconnectFromServer_) disconnectFromServer_();
  }

  void setConnectedNotifier(std::function<void(bool)> func) override {
    connectedCallback_ = func;
  }

  void setParamNotifier(std::function<void(const data::Value&)> func) override {
    setParamCallback_ = func;
  }

  QString getId(int index) override {
    return (getIdCallback_) ? getIdCallback_(index) : QString();
  }

 public:
  std::function<void(const QString&)> connectToServer_;
  std::function<void()> disconnectFromServer_;
  std::function<void(bool)> connectedCallback_;
  std::function<QString(int)> getIdCallback_;
  std::function<void(data::Value)> setParamCallback_;
};

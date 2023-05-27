#include "Client.h"

#include "GrpcClientAdapter.h"

Client::Client() : Client(std::make_unique<GrpcClientAdapter>()) {}

Client::Client(std::shared_ptr<IClientAdapter> adapter, QObject *parent)
    : QObject(parent), adapter_(std::move(adapter)) {
  address_.setBinding(
      [this]() { return QString("%1:%2").arg(ip_).arg(port_); });

  adapter_->setConnectedNotifier(
      [this](bool connected) { setConnected(connected); });
  adapter_->setParamNotifier(
      [this](const data::Value &value) { setParam(value); });
}

QString Client::ip() const { return ip_; }

void Client::setIp(const QString &ip) { ip_ = ip; }

int Client::port() const { return port_; }

void Client::setPort(int port) { port_ = port; }

bool Client::connected() const { return connected_; }

QBindable<bool> Client::bindableConnected() { return &connected_; }

void Client::connectToServer() { connectToServerInternal(address_); }

void Client::disconnectFromServer() { disconnectFromServerInternal(); }

QmlObjectListModel<ParamObject> *Client::model() { return &model_; }

void Client::connectToServerInternal(const QString &address) {
  if (!connected_ && adapter_ != nullptr) {
    adapter_->connectToServer(address);
  }
}

void Client::disconnectFromServerInternal() {
  if (connected_ && adapter_ != nullptr) {
    adapter_->disconnectFromServer();
  }
}

void Client::setConnected(bool connected) { connected_ = connected; }

void Client::updateParamIndex(int paramIndex) {
  if (!paramIndexToId_.contains(paramIndex)) {
    const auto &id = adapter_->getId(paramIndex);
    if (!id.isEmpty()) {
      paramIndexToId_[paramIndex] = id;
      auto paramObject = new ParamObject(&model_);
      paramObject->setIndex(paramIndex);
      model_.append(paramObject);
      updateParamId(*paramObject, paramIndex,
                    paramIndexToId_.value(paramIndex));
    }
  }
}

void Client::setParam(const data::Value &value) {
  updateParamIndex(value.index());
  updateParamModel(model_, value);
}

void updateParamModel(QmlObjectListModel<ParamObject> &model,
                      const data::Value &value) {
  for (const auto &paramObject : model) {
    updateParamValue(*paramObject, value.index(), value.value());
  }
}

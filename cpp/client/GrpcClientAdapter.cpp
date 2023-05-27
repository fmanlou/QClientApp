#include "GrpcClientAdapter.h"

#include <QGrpcChannel>
#include <QGrpcInsecureChannelCredentials>

GrpcClientAdapter::GrpcClientAdapter(Client *client)
    : grpcClient_(new data::RealtimeService::Client) {}

void GrpcClientAdapter::connectToServer(const QString &address) {
  channel_ = std::shared_ptr<QAbstractGrpcChannel>(new QGrpcChannel(
      QUrl(address), QGrpcChannel::InsecureChannelCredentials));

  grpcClient_->attachChannel(channel_);

  stream_ = grpcClient_->streamSendValue(data::Empty());

  QObject::connect(stream_.get(), &QGrpcStream::errorOccurred, stream_.get(),
                   [this]() {
                     if (setConnected_) setConnected_(false);
                   });

  QObject::connect(stream_.get(), &QGrpcStream::finished, stream_.get(),
                   [this]() {
                     if (setConnected_) setConnected_(false);
                   });

  QObject::connect(stream_.get(), &QGrpcStream::messageReceived, stream_.get(),
                   [this, stream = stream_.get()]() {
                     setConnected(true);
                     setParam(stream->read<data::Value>());
                   });
}

void GrpcClientAdapter::disconnectFromServer() {
  stream_.reset();
  channel_.reset();

  setConnected(false);
}

void GrpcClientAdapter::setConnectedNotifier(std::function<void(bool)> func) {
  setConnected_ = func;
}

void GrpcClientAdapter::setParamNotifier(
    std::function<void(const data::Value &)> func) {
  setParam_ = func;
}

QString GrpcClientAdapter::getId(int index) {
  data::Index request;
  data::Id response;
  request.setIndex(index);
  auto res = grpcClient_->getId(request, &response);
  if (res.code() == QGrpcStatus::Ok) {
    return response.id_proto();
  }
  return QString();
}

void GrpcClientAdapter::setConnected(bool connected) {
  if (setConnected_) setConnected_(connected);
}

void GrpcClientAdapter::setParam(const data::Value &value) {
  if (setParam_) setParam_(value);
}

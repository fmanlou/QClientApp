#pragma once

#include "Client.h"
#include "IClientAdapater.h"
#include "data.qpb.h"
#include "service_client.grpc.qpb.h"

class GrpcClientAdapter : public IClientAdapter {
 public:
  explicit GrpcClientAdapter(Client *client = nullptr);
  ~GrpcClientAdapter() = default;

  void connectToServer(const QString &address) override;
  void disconnectFromServer() override;
  void setConnectedNotifier(std::function<void(bool)> func) override;
  void setParamNotifier(std::function<void(const data::Value &)> func) override;
  QString getId(int index) override;

 private:
  void setConnected(bool connected);
  void setParam(const data::Value &value);

 private:
  std::shared_ptr<data::RealtimeService::Client> grpcClient_;
  std::shared_ptr<QAbstractGrpcChannel> channel_;
  std::shared_ptr<QGrpcStream> stream_;
  std::function<void(bool)> setConnected_;
  std::function<void(data::Value)> setParam_;
};
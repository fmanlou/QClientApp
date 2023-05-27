#pragma once

#include <QGrpcStatus>
#include <QGrpcStream>
#include <QString>

#include "data.qpb.h"
class IClientAdapter {
 public:
  virtual ~IClientAdapter() = default;
  virtual void connectToServer(const QString &address) = 0;
  virtual void disconnectFromServer() = 0;
  virtual void setConnectedNotifier(std::function<void(bool)> func) = 0;
  virtual void setParamNotifier(
      std::function<void(const data::Value &)> func) = 0;
  virtual QString getId(int index) = 0;
};

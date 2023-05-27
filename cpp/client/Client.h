#pragma once

#include <QObject>
#include <QProperty>
#include <QQmlEngine>
#include <QTimer>

#include "IClientAdapater.h"
#include "ParamObject.h"
#include "QmlObjectListModel.h"

constexpr auto kDefaultInterval = 1000;

class Client : public QObject {
  Q_OBJECT
  QML_ELEMENT
  Q_PROPERTY(QString ip READ ip WRITE setIp)
  Q_PROPERTY(int port READ port WRITE setPort)
  Q_PROPERTY(bool connected READ connected BINDABLE bindableConnected)
  Q_PROPERTY(QmlObjectListModel<ParamObject> *model READ model CONSTANT)

 public:
  Client();
  explicit Client(std::shared_ptr<IClientAdapter> adapter,
                  QObject *parent = nullptr);
  ~Client() = default;

  QString ip() const;
  void setIp(const QString &ip);
  int port() const;
  void setPort(int port);
  bool connected() const;
  QBindable<bool> bindableConnected();
  Q_INVOKABLE void connectToServer();
  Q_INVOKABLE void disconnectFromServer();
  QmlObjectListModel<ParamObject> *model();

 private:
  void connectToServerInternal(const QString &address);
  void disconnectFromServerInternal();
  void setConnected(bool connected);
  void updateParamIndex(int paramIndex);
  void setParam(const data::Value &value);

 private:
  QProperty<QString> ip_;
  QProperty<int> port_;
  QProperty<bool> connected_{false};
  QProperty<QString> address_;
  std::shared_ptr<IClientAdapter> adapter_;
  QMap<int, QString> paramIndexToId_;
  QmlObjectListModel<ParamObject> model_;
};

void updateParamModel(QmlObjectListModel<ParamObject> &model,
                      const data::Value &value);
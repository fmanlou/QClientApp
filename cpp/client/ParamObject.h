#pragma once

#include <QObject>
#include <QProperty>
#include <QQmlEngine>

class ParamObject : public QObject {
  Q_OBJECT
  QML_ELEMENT
  Q_PROPERTY(int index READ index NOTIFY indexChanged BINDABLE bindableIndex)
  Q_PROPERTY(int value READ value NOTIFY valueChanged BINDABLE bindableValue)
  Q_PROPERTY(QString id READ id NOTIFY idChanged BINDABLE bindableId)

 public:
  ParamObject(QObject *parent = nullptr);
  ~ParamObject() = default;

  int index() const;
  void setIndex(int index);
  QBindable<int> bindableIndex();
  int value() const;
  void setValue(int value);
  QBindable<int> bindableValue();
  QString id() const;
  void setId(const QString &id);
  QBindable<QString> bindableId();

 signals:
  void indexChanged();
  void valueChanged();
  void idChanged();

 private:
  Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(ParamObject, int, index_, 0,
                                       &ParamObject::indexChanged);
  Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(ParamObject, int, value_, 0,
                                       &ParamObject::valueChanged);
  Q_OBJECT_BINDABLE_PROPERTY(ParamObject, QString, id_,
                             &ParamObject::idChanged);
};

void updateParamValue(ParamObject &object, int index, int value);

void updateParamId(ParamObject &object, int index, QString id);
#include "ParamObject.h"

ParamObject::ParamObject(QObject *parent) : QObject(parent) {}

int ParamObject::index() const { return index_; }

void ParamObject::setIndex(int index) { index_ = index; }

QBindable<int> ParamObject::bindableIndex() { return &index_; }

int ParamObject::value() const { return value_; }

void ParamObject::setValue(int value) { value_ = value; }

QBindable<int> ParamObject::bindableValue() { return &value_; }

QString ParamObject::id() const { return id_; }

void ParamObject::setId(const QString &id) { id_ = id; }

QBindable<QString> ParamObject::bindableId() { return &id_; }

void updateParamValue(ParamObject &object, int index, int value) {
  if (object.index() == index) {
    object.setValue(value);
  }
}

void updateParamId(ParamObject &object, int index, QString id) {
  if (object.index() == index) {
    object.setId(id);
  }
}

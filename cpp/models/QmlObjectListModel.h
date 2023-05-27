#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QMetaMethod>
#include <QMetaObject>

class IQmlObjectListModel : public QAbstractListModel {
  Q_OBJECT
 public:
  explicit IQmlObjectListModel(QObject *parent = nullptr)
      : QAbstractListModel(parent) {}
  virtual ~IQmlObjectListModel() = default;

 public:
  virtual void append(QObject *object) = 0;

 protected slots:
  virtual void onObjectPropertyChanged() = 0;
};

struct MetaRole {
  int role;
  QByteArray name;
  std::optional<int> notifySignalIndex;

  bool operator==(const MetaRole &other) const {
    return role == other.role && name == other.name &&
           notifySignalIndex == other.notifySignalIndex;
  }
};

template <class TObject>
TObject *at(int index, const QList<TObject *> &objects) {
  if (index < 0 || index >= objects.size()) {
    return nullptr;
  }
  return objects.at(index);
}

using FBeginRemoveRows = std::function<void(const QModelIndex &, int, int)>;
using FEndRemoveRows = std::function<void()>;

template <class TObject>
void removeAt(TObject *object, QList<TObject *> &objects,
              FBeginRemoveRows begin, FEndRemoveRows end) {
  auto index = objects.indexOf(object);
  if (index < 0) return;
  if (begin) begin(QModelIndex(), index, index);
  objects.removeAt(index);
  if (end) end();
}

void getNotifySignalIndexs(const QList<MetaRole> &metaObject,
                           QHash<int, int> &indexs);

void getRoleNames(const QList<MetaRole> &metaObject,
                  QHash<int, QByteArray> &roleNames);

void getMetaRoles(const QMetaObject &metaObject, QList<MetaRole> &metaRoles);

template <class TObject>
class QmlObjectListModel : public IQmlObjectListModel {
 public:
  explicit QmlObjectListModel(QObject *parent = nullptr)
      : IQmlObjectListModel(parent) {
    getMetaRoles(TObject::staticMetaObject, metaRoles_);
    getRoleNames(metaRoles_, roleNames_);
    getNotifySignalIndexs(metaRoles_, notifySignalIndexs_);
    handler_ = metaObject()->method(
        metaObject()->indexOfMethod("onObjectPropertyChanged()"));
  }

  bool setData(const QModelIndex &index, const QVariant &value,
               int role) override {
    auto object = at(index.row(), objects_);
    if (object == nullptr) return false;
    auto name = roleNames_.value(role);
    if (name.isEmpty()) return false;
    object->setProperty(name, value);
    return true;
  }

  QVariant data(const QModelIndex &index, int role) const override {
    auto object = at(index.row(), objects_);
    if (object == nullptr) return QVariant();
    auto name = roleNames_.value(role);
    if (name.isEmpty()) return QVariant();
    return object->property(name);
  }

  QHash<int, QByteArray> roleNames() const override { return roleNames_; }

  int rowCount(const QModelIndex &parent) const override {
    return objects_.size();
  }

  void append(QObject *object) override {
    beginInsertRows(QModelIndex(), objects_.size(), objects_.size());
    objects_.append(qobject_cast<TObject *>(object));
    attachObject(qobject_cast<TObject *>(object));
    endInsertRows();
  }

  auto begin() { return objects_.begin(); }

  auto end() { return objects_.end(); }

  auto constBegin() const { return objects_.constBegin(); }

  auto constEnd() const { return objects_.constEnd(); }

 protected:
  void attachObject(TObject *object) {
    if (object == nullptr) return;
    connect(object, &QObject::destroyed, this, [this, object] {
      removeAt(
          object, objects_,
          [this](const QModelIndex &, int first, int last) {
            beginRemoveRows(QModelIndex(), first, last);
          },
          [this]() { endRemoveRows(); });
    });
    for (const auto &key : notifySignalIndexs_.keys()) {
      connect(object, object->metaObject()->method(key), this, handler_);
    }
  }

  void detachObject(TObject *object) {
    if (object == nullptr) return;
    disconnect(this, nullptr, object, nullptr);
    disconnect(object, nullptr, this, nullptr);
  }

  void onObjectPropertyChanged() override {
    auto object = qobject_cast<TObject *>(sender());
    if (object == nullptr) return;
    auto row = objects_.indexOf(object);
    if (row < 0) return;
    auto sig = senderSignalIndex();
    auto role = notifySignalIndexs_.value(sig);
    if (role < 0) return;
    const QModelIndex index = createIndex(row, 0);
    emit dataChanged(index, index, QVector<int>{role});
  }

 private:
  QList<TObject *> objects_;
  QHash<int, QByteArray> roleNames_;
  QList<MetaRole> metaRoles_;
  QHash<int, int> notifySignalIndexs_;
  QMetaMethod handler_;
};
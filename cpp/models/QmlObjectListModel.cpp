#include "QmlObjectListModel.h"

#include <QMetaProperty>

void getNotifySignalIndexs(const QList<MetaRole> &metaObject,
                           QHash<int, int> &indexs) {
  for (const auto &metaRole : metaObject) {
    if (metaRole.notifySignalIndex.has_value()) {
      indexs.insert(metaRole.notifySignalIndex.value(), metaRole.role);
    }
  }
}

void getRoleNames(const QList<MetaRole> &metaObject,
                  QHash<int, QByteArray> &roleNames) {
  for (const auto &metaRole : metaObject) {
    roleNames.insert(metaRole.role, metaRole.name);
  }
}

void getMetaRoles(const QMetaObject &metaObject, QList<MetaRole> &metaRoles) {
  metaRoles.clear();
  for (auto i = 0; i < metaObject.propertyCount(); ++i) {
    const auto &property = metaObject.property(i);
    const auto &name = property.name();
    auto role = Qt::UserRole + i + 1;
    std::optional<int> notifySignalIndex = std::nullopt;
    if (property.hasNotifySignal()) {
      notifySignalIndex = property.notifySignalIndex();
    }
    metaRoles.append({role, name, notifySignalIndex});
  }
}

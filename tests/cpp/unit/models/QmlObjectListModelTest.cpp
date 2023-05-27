#include <catch2/catch_all.hpp>

#include "ParamObject.h"
#include "QmlObjectListModel.h"

struct QmlObjectListModelTest {
  void testAt() {
    QList<ParamObject *> objects;
    ParamObject object1;
    objects.append(&object1);
    REQUIRE(at(0, objects) == &object1);
    REQUIRE(at(1, objects) == nullptr);
    REQUIRE(at(-1, objects) == nullptr);
  }

  void testRemoveAt() {
    QList<ParamObject *> objects;
    ParamObject object1;
    objects.append(&object1);
    FBeginRemoveRows begin = [](const QModelIndex &parent, int first,
                                int last) {
      REQUIRE(first == 0);
      REQUIRE(last == 0);
    };
    FEndRemoveRows end = []() {};
    removeAt(&object1, objects, nullptr, nullptr);
    removeAt(&object1, objects, begin, end);
    REQUIRE(objects.size() == 0);
  }

  void testRemoveAt2() {
    bool called = false;
    QList<ParamObject *> objects;
    FBeginRemoveRows begin = [&](const QModelIndex &parent, int first,
                                 int last) { called = true; };
    FEndRemoveRows end = [&]() { called = true; };
    removeAt((ParamObject *)nullptr, objects, begin, end);
    REQUIRE(called == false);
  }

  void testGetMetaRoles() {
    QList<MetaRole> metaRoles;
    getMetaRoles(ParamObject::staticMetaObject, metaRoles);
    REQUIRE(metaRoles.size() == 4);
    REQUIRE(metaRoles[0] == MetaRole{Qt::UserRole + 1, "objectName", 2});
    REQUIRE(metaRoles[1] == MetaRole{Qt::UserRole + 2, "index", 5});
    REQUIRE(metaRoles[2] == MetaRole{Qt::UserRole + 3, "value", 6});
    REQUIRE(metaRoles[3] == MetaRole{Qt::UserRole + 4, "id", 7});
  }

  void testGetRoleNames() {
    QList<MetaRole> metaRoles;
    metaRoles.append(MetaRole{Qt::UserRole + 1, "objectName", 2});
    QHash<int, QByteArray> roleNames;
    getRoleNames(metaRoles, roleNames);
    REQUIRE(roleNames.size() == 1);
    REQUIRE(roleNames[Qt::UserRole + 1] == "objectName");
  }

  void testGetNotifySignalIndexs() {
    QList<MetaRole> metaRoles;
    metaRoles.append(MetaRole{Qt::UserRole + 1, "objectName", 2});
    metaRoles.append(MetaRole{Qt::UserRole + 2, "index", std::nullopt});
    QHash<int, int> notifySignalIndexs;
    getNotifySignalIndexs(metaRoles, notifySignalIndexs);
    REQUIRE(notifySignalIndexs.size() == 1);
    REQUIRE(notifySignalIndexs[2] == Qt::UserRole + 1);
  }

  void testSetData() {
    ParamObject object1;
    QmlObjectListModel<ParamObject> model;
    REQUIRE(model.setData(model.index(0, 0), "test", Qt::UserRole + 1) ==
            false);
    model.append(&object1);
    REQUIRE(model.setData(model.index(0, 0), "test", Qt::UserRole + 100) ==
            false);
    REQUIRE(model.setData(model.index(0, 0), "test", Qt::UserRole + 1));
    REQUIRE(object1.objectName() == "test");
  }

  void testData() {
    ParamObject object1;
    object1.setId("object1");
    QmlObjectListModel<ParamObject> model;
    REQUIRE(model.data(model.index(0, 0), Qt::UserRole + 1).isNull());
    REQUIRE(model.data(model.index(0, 0), Qt::UserRole + 100).isNull());
    model.append(&object1);
    REQUIRE(model.data(model.index(0, 0), Qt::UserRole + 4) == object1.id());
  }

  void testRoleNames() {
    QmlObjectListModel<ParamObject> model;
    QHash<int, QByteArray> roleNames = model.roleNames();
    REQUIRE(roleNames.size() == 4);
    REQUIRE(roleNames[Qt::UserRole + 1] == "objectName");
    REQUIRE(roleNames[Qt::UserRole + 2] == "index");
    REQUIRE(roleNames[Qt::UserRole + 3] == "value");
    REQUIRE(roleNames[Qt::UserRole + 4] == "id");
  }

  void testRowCount() {
    QmlObjectListModel<ParamObject> model;
    REQUIRE(model.rowCount(QModelIndex()) == 0);
    {
      ParamObject object1;
      model.append(&object1);
      REQUIRE(model.rowCount(QModelIndex()) == 1);
    }
    REQUIRE(model.rowCount(QModelIndex()) == 0);
  }

  void testIterator() {
    QmlObjectListModel<ParamObject> model;
    ParamObject object1;
    model.append(&object1);
    for (auto it = model.begin(); it != model.end(); ++it) {
      REQUIRE(*it == &object1);
    }
    for (auto it = model.constBegin(); it != model.constEnd(); ++it) {
      REQUIRE(*it == &object1);
    }
  }
};

TEST_CASE_METHOD(QmlObjectListModelTest, "[QmlObjectListModel]") {
  SECTION("testAt") { testAt(); }
  SECTION("testRemoveAt") {
    testRemoveAt();
    testRemoveAt2();
  }
  SECTION("testGetMetaRoles") { testGetMetaRoles(); }
  SECTION("testGetRoleNames") { testGetRoleNames(); }
  SECTION("testGetNotifySignalIndexs") { testGetNotifySignalIndexs(); }
  SECTION("testSetData") { testSetData(); }
  SECTION("testData") { testData(); }
  SECTION("testRoleNames") { testRoleNames(); }
  SECTION("testRowCount") { testRowCount(); }
  SECTION("testIterator") { testIterator(); }
}

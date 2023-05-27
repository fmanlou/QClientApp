#include <catch2/catch_all.hpp>

#include "ParamObject.h"

struct ParamObjectTest {
  void testBindableIndex() {
    ParamObject object;
    auto bindableIndex = object.bindableIndex();
    REQUIRE(bindableIndex.value() == 0);
    object.setIndex(1);
    REQUIRE(bindableIndex.value() == 1);
  }

  void testBindableValue() {
    ParamObject object;
    auto bindableValue = object.bindableValue();
    REQUIRE(bindableValue.value() == 0);
    object.setValue(1);
    REQUIRE(bindableValue.value() == 1);
  }

  void testBindableId() {
    ParamObject object;
    auto bindableId = object.bindableId();
    REQUIRE(bindableId.value() == "");
    object.setId("id");
    REQUIRE(bindableId.value() == "id");
  }

  void testUpdateParamValue() {
    ParamObject object;
    object.setIndex(1);
    object.setValue(2);
    updateParamValue(object, 1, 3);
    REQUIRE(object.value() == 3);
    updateParamValue(object, 2, 3);
    REQUIRE(object.value() == 3);
  }

  void testUpdateParamId() {
    ParamObject object;
    object.setIndex(1);
    object.setId("id");
    updateParamId(object, 1, "newId");
    REQUIRE(object.id() == "newId");
    updateParamId(object, 2, "otherId");
    REQUIRE(object.id() == "newId");
  }
};

TEST_CASE_METHOD(ParamObjectTest, "updateParamValue") {
  SECTION("testBindableIndex") { testBindableIndex(); }
  SECTION("testBindableValue") { testBindableValue(); }
  SECTION("testBindableId") { testBindableId(); }
  SECTION("updateParamValue") { testUpdateParamValue(); }
  SECTION("updateParamId") { testUpdateParamId(); }
}
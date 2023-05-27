#include <QCoreApplication>
#include <QTimer>
#include <catch2/catch_all.hpp>

#include "Client.h"
#include "ClientAdapterMock.h"

struct ClientTest {
  ClientTest() {
    adapter_ = std::make_shared<ClientAdapterMock>();
    client_ = std::make_shared<Client>(adapter_);
  }

  void testIp() {
    REQUIRE(client_->ip() == "");
    client_->setIp("localhost");
    REQUIRE(client_->ip() == "localhost");
  }

  void testPort() {
    REQUIRE(client_->port() == 0);
    client_->setPort(50051);
    REQUIRE(client_->port() == 50051);
  }

  void testConnectToServer() {
    adapter_->connectToServer_ = [this](const QString &address) {
      REQUIRE(address == "localhost:50051");
      adapter_->connectedCallback_(true);
    };
    auto bindableConnected = client_->bindableConnected();
    client_->setIp("localhost");
    client_->setPort(50051);
    client_->connectToServer();
    REQUIRE(client_->connected() == true);
    REQUIRE(bindableConnected.value() == true);
  }

  std::shared_ptr<ClientAdapterMock> adapter_;
  std::shared_ptr<Client> client_;
};

TEST_CASE_METHOD(ClientTest, "[Client]") {
  SECTION("testIp") { testIp(); }
  SECTION("testPort") { testPort(); }
  SECTION("testConnectToServer") { testConnectToServer(); }
}

TEST_CASE("connect", "[Client]") {
  auto adapter = std::make_shared<ClientAdapterMock>();
  auto adapterPtr = adapter.get();
  adapterPtr->disconnectFromServer_ = [adapterPtr]() {
    adapterPtr->connectedCallback_(false);
  };

  Client client(std::move(adapter));

  adapterPtr->connectToServer_ = [adapterPtr](const QString &address) {
    REQUIRE(address == "localhost:50051");
    adapterPtr->connectedCallback_(true);
  };
  client.setIp("localhost");
  client.setPort(50051);
  client.connectToServer();
  REQUIRE(client.connected() == true);
  client.disconnectFromServer();
  REQUIRE(client.connected() == false);

  adapterPtr->connectToServer_ = [adapterPtr](const QString &address) {
    REQUIRE(address == "127.0.0.1:50052");
  };
  client.setIp("127.0.0.1");
  client.setPort(50052);
  client.connectToServer();
}

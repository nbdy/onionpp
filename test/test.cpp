//
// Created by nbdy on 05.02.25.
//

#include <gtest/gtest.h>
#include "onionpp/Tor/Tor.h"

class TestFixture : public ::testing::Test {
  protected:
  onionpp::Tor* m_pTor = nullptr;

  void SetUp() override {
    m_pTor = new onionpp::Tor();
  }

  void TearDown() override {
    delete m_pTor;
  }

  [[nodiscard]] bool isBootstrapped() const {
    return m_pTor->isBootstrapped();
  }
};

TEST_F(TestFixture, All) {
  EXPECT_FALSE(m_pTor->isRunning());
  m_pTor->start(false);
  EXPECT_TRUE(m_pTor->isRunning());
  EXPECT_FALSE(m_pTor->isBootstrapped());
  m_pTor->waitUntilBootstrapped();
  EXPECT_TRUE(m_pTor->isBootstrapped());
  m_pTor->stop();
  EXPECT_FALSE(m_pTor->isRunning());
  EXPECT_FALSE(m_pTor->isBootstrapped());
}

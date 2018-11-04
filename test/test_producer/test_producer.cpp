#include "gtest/gtest.h"
#include <array>

namespace {

class ProducerTest : public ::testing::Test{

protected:

  ProducerTest(){}

  void SetUp() override {

  }

  void TearDown() override{

  }

};

TEST_F(ProducerTest, DoesConstruct)
{


    ASSERT_TRUE(true);
}

}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

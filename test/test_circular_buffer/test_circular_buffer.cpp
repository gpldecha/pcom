#include "gtest/gtest.h"
#include "pcom/spsc_circular_buffer.h"
#include <boost/asio.hpp>
#include <array>


using namespace pcom;

namespace {

  class CircularBufferTest : public ::testing::Test{

protected:

  CircularBufferTest(){}

  void SetUp() override {

  }

  void TearDown() override{

  }

};

TEST_F(CircularBufferTest, RandomAccessOperator)
{

  std::array<int, 1> item1 = {1};
  std::array<int, 1> item2 = {2};

  CircularBuffer circular_buffer(CircularBuffer::QueueSize(2), CircularBuffer::ItemSizeBytes(boost::asio::buffer_size(boost::asio::buffer(item1))));
  circular_buffer.push(boost::asio::buffer(item1));
  circular_buffer.push(boost::asio::buffer(item2));

  boost::asio::const_buffer b1 = circular_buffer[0];
  boost::asio::const_buffer b2 = circular_buffer[1];

  std::array<int, 1> const* out1 = static_cast<std::array<int, 1> const*>(boost::asio::buffer_cast<const void*>(b1));
  std::array<int, 1> const* out2 = static_cast<std::array<int, 1> const*>(boost::asio::buffer_cast<const void*>(b2));

  int num1 = (*out1)[0];
  int num2 = (*out2)[0];

  ASSERT_EQ(num1, item1[0]);
  ASSERT_EQ(num2, item2[0]);

}

TEST_F(CircularBufferTest, InsertMoreThanCapacity)
{

  std::array<int, 1> item1 = {1};
  std::array<int, 1> item2 = {2};
  std::array<int, 1> item3 = {3};

  CircularBuffer circular_buffer(CircularBuffer::QueueSize(2), CircularBuffer::ItemSizeBytes(boost::asio::buffer_size(boost::asio::buffer(item1))));
  circular_buffer.push(boost::asio::buffer(item1));
  circular_buffer.push(boost::asio::buffer(item2));
  circular_buffer.push(boost::asio::buffer(item3));

  boost::asio::const_buffer b1 = circular_buffer[0];
  boost::asio::const_buffer b2 = circular_buffer[1];

  std::array<int, 1> const* out1 = static_cast<std::array<int, 1> const*>(boost::asio::buffer_cast<const void*>(b1));
  std::array<int, 1> const* out2 = static_cast<std::array<int, 1> const*>(boost::asio::buffer_cast<const void*>(b2));

  int num1 = (*out1)[0];
  int num2 = (*out2)[0];

  ASSERT_EQ(num1, item3[0]);
  ASSERT_EQ(num2, item2[0]);

}

TEST_F(CircularBufferTest, TestPop)
{
  std::array<int, 1> item1 = {1};
  std::array<int, 1> item2 = {2};

  CircularBuffer circular_buffer(CircularBuffer::QueueSize(2), CircularBuffer::ItemSizeBytes(boost::asio::buffer_size(boost::asio::buffer(item1))));
  circular_buffer.push(boost::asio::buffer(item1));
  circular_buffer.push(boost::asio::buffer(item2));

  boost::asio::mutable_buffer b(new unsigned char[4], 4);
  circular_buffer.pop(b);

  std::array<int, 1> const* out1 = static_cast<std::array<int, 1> const*>(boost::asio::buffer_cast<const void*>(b));
  int num1 = (*out1)[0];

  ASSERT_EQ(num1, item1[0]);
  ASSERT_EQ((int)circular_buffer.size(), 1);

  circular_buffer.pop(b);
  std::array<int, 1> const* out2 = static_cast<std::array<int, 1> const*>(boost::asio::buffer_cast<const void*>(b));
  int num2 = (*out2)[0];

  ASSERT_EQ(num2, item2[0]);
  ASSERT_EQ((int)circular_buffer.size(), 0);

}

TEST_F(CircularBufferTest, TestLargeInArrayItemRandomAcessOperator){
  std::array<int, 1000> item1; std::fill_n(item1.begin(), item1.size(), 1);
  std::array<int, 1000> item2; std::fill_n(item2.begin(), item2.size(), 2);

  CircularBuffer circular_buffer(CircularBuffer::QueueSize(2), CircularBuffer::ItemSizeBytes(boost::asio::buffer_size(boost::asio::buffer(item1))));
  circular_buffer.push(boost::asio::buffer(item1));
  circular_buffer.push(boost::asio::buffer(item2));

  boost::asio::const_buffer b1 = circular_buffer[0];
  boost::asio::const_buffer b2 = circular_buffer[1];

  std::array<int, 1000> const* out1 = static_cast<std::array<int, 1000> const*>(boost::asio::buffer_cast<const void*>(b1));
  std::array<int, 1000> const* out2 = static_cast<std::array<int, 1000> const*>(boost::asio::buffer_cast<const void*>(b2));

  for(std::size_t i = 0; i < 1000; i++){
    ASSERT_EQ((*out1)[i], 1);
    ASSERT_EQ((*out2)[i], 2);
  }
}

TEST_F(CircularBufferTest, TestLargeFloatArrayItemRandomAcessOperator){
  std::array<double, 1000> item1; std::fill_n(item1.begin(), item1.size(), 1);
  std::array<double, 1000> item2; std::fill_n(item2.begin(), item2.size(), 2);

  CircularBuffer circular_buffer(CircularBuffer::QueueSize(2), CircularBuffer::ItemSizeBytes(boost::asio::buffer_size(boost::asio::buffer(item1))));
  circular_buffer.push(boost::asio::buffer(item1));
  circular_buffer.push(boost::asio::buffer(item2));

  boost::asio::const_buffer b1 = circular_buffer[0];
  boost::asio::const_buffer b2 = circular_buffer[1];

  std::array<double, 1000> const* out1 = static_cast<std::array<double, 1000> const*>(boost::asio::buffer_cast<const void*>(b1));
  std::array<double, 1000> const* out2 = static_cast<std::array<double, 1000> const*>(boost::asio::buffer_cast<const void*>(b2));

  for(std::size_t i = 0; i < 1000; i++){
    ASSERT_EQ((*out1)[i], 1);
    ASSERT_EQ((*out2)[i], 2);
  }
}

}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include <config.h>

#include <gtest/gtest.h>
#include <primitiv/c/naive_device.h>

namespace primitiv_c {

class CDeviceTest : public testing::Test {};

TEST_F(CDeviceTest, CheckDefault) {
  ::primitiv_Device *device;
  EXPECT_EQ(::primitiv_Status::PRIMITIV_ERROR,
            ::primitiv_Device_get_default(&device));
  {
    ::primitiv_Device *dev1 = ::primitiv_Naive_new();
    ::primitiv_Device_set_default(dev1);
    ::primitiv_Device_get_default(&device);
    EXPECT_EQ(dev1, device);
    {
      ::primitiv_Device *dev2 = ::primitiv_Naive_new();
      ::primitiv_Device_set_default(dev2);
      ::primitiv_Device_get_default(&device);
      EXPECT_EQ(dev2, device);
      ::primitiv_Naive_delete(dev2);
    }
    EXPECT_EQ(::primitiv_Status::PRIMITIV_ERROR,
              ::primitiv_Device_get_default(&device));
    ::primitiv_Device *dev3 = ::primitiv_Naive_new();
    ::primitiv_Device_set_default(dev3);
    ::primitiv_Device_get_default(&device);
    EXPECT_EQ(dev3, device);
    ::primitiv_Naive_delete(dev1);
    ::primitiv_Naive_delete(dev3);
  }
  EXPECT_EQ(::primitiv_Status::PRIMITIV_ERROR,
            ::primitiv_Device_get_default(&device));
}

}  // namespace primitiv_c

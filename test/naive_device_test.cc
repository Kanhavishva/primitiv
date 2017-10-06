#include <config.h>

#include <chrono>
#include <stdexcept>
#include <thread>
#include <vector>
#include <gtest/gtest.h>
#include <primitiv/naive_device.h>
#include <primitiv/shape.h>
#include <primitiv/tensor.h>
#include <test_utils.h>

using std::vector;
using test_utils::vector_match;

namespace primitiv {

class NaiveDeviceTest : public testing::Test {};

TEST_F(NaiveDeviceTest, CheckDeviceType) {
  devices::Naive dev;
  EXPECT_EQ(Device::DEVICE_TYPE_CPU, dev.type());
}

TEST_F(NaiveDeviceTest, CheckRandomBernoulli) {
  vector<vector<float>> history;
  for (unsigned i = 0; i < 10; ++i) {
    devices::Naive dev;
    const Tensor x = dev.random_bernoulli(Shape({3, 3}, 3), 0.3);
    const vector<float> x_val = x.to_vector();

    std::cout << "Epoch " << i << ':';
    for (float x_i : x_val) {
      std::cout << ' ' << x_i;
    }
    std::cout << std::endl;

    for (const vector<float> &h_val : history) {
      EXPECT_FALSE(vector_match(x_val, h_val));
    }
    history.emplace_back(x_val);

    // Wait for updating the device randomizer.
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
}

TEST_F(NaiveDeviceTest, CheckRandomBernoulliWithSeed) {
  const vector<float> expected {
    0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0,
  };
  devices::Naive dev(12345);
  const Tensor x = dev.random_bernoulli(Shape({4, 4}, 4), 0.3);
  EXPECT_TRUE(vector_match(expected, x.to_vector()));
}

TEST_F(NaiveDeviceTest, CheckRandomUniform) {
  vector<vector<float>> history;
  for (unsigned i = 0; i < 10; ++i) {
    devices::Naive dev;
    const Tensor x = dev.random_uniform(Shape({2, 2}, 2), -9, 9);
    const vector<float> x_val = x.to_vector();

    std::cout << "Epoch " << i << ':';
    for (float x_i : x_val) {
      std::cout << ' ' << x_i;
    }
    std::cout << std::endl;

    for (const vector<float> &h_val : history) {
      EXPECT_FALSE(vector_match(x_val, h_val));
    }
    history.emplace_back(x_val);

    // Wait for updating the device randomizer.
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
}

TEST_F(NaiveDeviceTest, CheckRandomUniformWithSeed) {
  const vector<float> expected {
    7.7330894e+00, 7.0227852e+00, -3.3052402e+00, -6.6472688e+00,
    -5.6894612e+00, -8.2843294e+00, -5.3179150e+00, 5.8758497e+00,
  };
  devices::Naive dev(12345);
  const Tensor x = dev.random_uniform(Shape({2, 2}, 2), -9, 9);
  EXPECT_TRUE(vector_match(expected, x.to_vector()));
}

TEST_F(NaiveDeviceTest, CheckRandomNormal) {
  vector<vector<float>> history;
  for (unsigned i = 0; i < 10; ++i) {
    devices::Naive dev;
    const Tensor x = dev.random_normal(Shape({2, 2}, 2), 1, 3);
    const vector<float> x_val = x.to_vector();

    std::cout << "Epoch " << i << ':';
    for (float x_i : x_val) {
      std::cout << ' ' << x_i;
    }
    std::cout << std::endl;

    for (const vector<float> &h_val : history) {
      EXPECT_FALSE(vector_match(x_val, h_val));
    }
    history.emplace_back(x_val);

    // Wait for updating the device randomizer.
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
}

TEST_F(NaiveDeviceTest, CheckRandomNormalWithSeed) {
  const vector<float> expected {
    -1.3574908e+00, -1.7222166e-01, 2.5865970e+00, -4.3594337e-01,
    4.5383353e+00, 8.4703674e+00, 2.5535507e+00, 1.3252910e+00,
  };
  devices::Naive dev(12345);
  const Tensor x = dev.random_normal(Shape({2, 2}, 2), 1, 3);
  EXPECT_TRUE(vector_match(expected, x.to_vector()));
}

TEST_F(NaiveDeviceTest, CheckRandomLogNormal) {
  vector<vector<float>> history;
  for (unsigned i = 0; i < 10; ++i) {
    devices::Naive dev;
    const Tensor x = dev.random_log_normal(Shape({2, 2}, 2), 1, 3);
    const vector<float> x_val = x.to_vector();

    std::cout << "Epoch " << i << ':';
    for (float x_i : x_val) {
      std::cout << ' ' << x_i;
    }
    std::cout << std::endl;

    for (const vector<float> &h_val : history) {
      EXPECT_FALSE(vector_match(x_val, h_val));
    }
    history.emplace_back(x_val);

    // Wait for updating the device randomizer.
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
}

TEST_F(NaiveDeviceTest, CheckRandomLogNormalWithSeed) {
  const vector<float> expected {
    2.5730559e-01, 8.4179258e-01, 1.3284487e+01, 6.4665437e-01,
    9.3534966e+01, 4.7712681e+03, 1.2852659e+01, 3.7632804e+00,
  };
  devices::Naive dev(12345);
  const Tensor x = dev.random_log_normal(Shape({2, 2}, 2), 1, 3);
  EXPECT_TRUE(vector_match(expected, x.to_vector()));
}

}  // namespace primitiv

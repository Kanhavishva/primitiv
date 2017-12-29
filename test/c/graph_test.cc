#include <primitiv/config.h>

#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include <primitiv/c/functions.h>
#include <primitiv/c/graph.h>
#include <primitiv/c/naive_device.h>
#include <primitiv/c/tensor.h>
#include <test_utils.h>

using std::vector;
using test_utils::array_match;

namespace primitiv {
namespace c {

class CGraphTest : public testing::Test {
  void SetUp() override {
    ::primitiv_devices_Naive_new(&dev);
    ::primitiv_devices_Naive_new(&dev2);
  }
  void TearDown() override {
    ::primitiv_Device_delete(dev);
    ::primitiv_Device_delete(dev2);
  }
 protected:
  ::primitiv_Device *dev;
  ::primitiv_Device *dev2;
};

TEST_F(CGraphTest, CheckDefault) {
  ::primitiv_Status_reset();
  ::primitiv_Graph *graph;
  EXPECT_EQ(::primitiv_Status::PRIMITIV_ERROR,
            ::primitiv_Graph_get_default(&graph));
  {
    ::primitiv_Graph *g1;
    ASSERT_EQ(::primitiv_Status::PRIMITIV_OK, ::primitiv_Graph_new(&g1));
    ::primitiv_Graph_set_default(g1);
    ::primitiv_Graph_get_default(&graph);
    EXPECT_EQ(g1, graph);
    {
      ::primitiv_Graph *g2;
      ASSERT_EQ(::primitiv_Status::PRIMITIV_OK, ::primitiv_Graph_new(&g2));
      ::primitiv_Graph_set_default(g2);
      ::primitiv_Graph_get_default(&graph);
      EXPECT_EQ(g2, graph);
      ::primitiv_Graph_delete(g2);
    }
    EXPECT_EQ(::primitiv_Status::PRIMITIV_ERROR,
              ::primitiv_Graph_get_default(&graph));
    ::primitiv_Graph *g3;
    ASSERT_EQ(::primitiv_Status::PRIMITIV_OK, ::primitiv_Graph_new(&g3));
    ::primitiv_Graph_set_default(g3);
    ::primitiv_Graph_get_default(&graph);
    EXPECT_EQ(g3, graph);
    ::primitiv_Graph_delete(g1);
    ::primitiv_Graph_delete(g3);
  }
  EXPECT_EQ(::primitiv_Status::PRIMITIV_ERROR,
            ::primitiv_Graph_get_default(&graph));
}

TEST_F(CGraphTest, CheckInvalidNode) {
  ::primitiv_Status_reset();
  ::primitiv_Node *node;
  ASSERT_EQ(::primitiv_Status::PRIMITIV_OK, ::primitiv_Node_new(&node));
  unsigned char valid;
  ::primitiv_Node_valid(node, &valid);
  EXPECT_FALSE(valid);
  ::primitiv_Graph *graph;
  EXPECT_EQ(::primitiv_Status::PRIMITIV_ERROR,
            ::primitiv_Node_graph(node, &graph));
  uint32_t id;
  EXPECT_EQ(::primitiv_Status::PRIMITIV_ERROR,
            ::primitiv_Node_operator_id(node, &id));
  EXPECT_EQ(::primitiv_Status::PRIMITIV_ERROR,
            ::primitiv_Node_value_id(node, &id));
  ::primitiv_Shape *shape;
  EXPECT_EQ(::primitiv_Status::PRIMITIV_ERROR,
            ::primitiv_Node_shape(node, &shape));
  ::primitiv_Device *device;
  EXPECT_EQ(::primitiv_Status::PRIMITIV_ERROR,
            ::primitiv_Node_device(node, &device));
  float value;
  EXPECT_EQ(::primitiv_Status::PRIMITIV_ERROR,
            ::primitiv_Node_to_float(node, &value));
  std::size_t num_values = 20;
  float values[num_values];
  EXPECT_EQ(::primitiv_Status::PRIMITIV_ERROR,
            ::primitiv_Node_to_array(node, values, &num_values));
  EXPECT_EQ(::primitiv_Status::PRIMITIV_ERROR,
            ::primitiv_Node_backward(node));
  ::primitiv_Node_delete(node);
}

TEST_F(CGraphTest, CheckClear) {
  ::primitiv_Status_reset();
  ::primitiv_Device_set_default(dev);

  ::primitiv_Graph *g;
  ASSERT_EQ(::primitiv_Status::PRIMITIV_OK, ::primitiv_Graph_new(&g));
  ::primitiv_Graph_set_default(g);

  uint32_t num;
  ::primitiv_Graph_num_operators(g, &num);
  EXPECT_EQ(0u, num);

  {
    ::primitiv_Shape *shape;
    ASSERT_EQ(::primitiv_Status::PRIMITIV_OK, ::primitiv_Shape_new(&shape));
    float values[] = {1};
    ::primitiv_Node *node1;
    ::primitiv_node_func_input(shape, values, 1, nullptr, nullptr, &node1);
    ::primitiv_Node_delete(node1);
    ::primitiv_Node *node2;
    ::primitiv_node_func_input(shape, values, 1, nullptr, nullptr, &node2);
    ::primitiv_Node_delete(node2);
    ::primitiv_Graph_num_operators(g, &num);
    EXPECT_EQ(2u, num);
  }

  ::primitiv_Graph_clear(g);
  ::primitiv_Graph_num_operators(g, &num);
  EXPECT_EQ(0u, num);

  {
    ::primitiv_Shape *shape;
    ASSERT_EQ(::primitiv_Status::PRIMITIV_OK, ::primitiv_Shape_new(&shape));
    float values[] = {1};
    ::primitiv_Node *node;
    ::primitiv_node_func_input(shape, values, 1, nullptr, nullptr, &node);
    ::primitiv_Node_delete(node);
    ::primitiv_node_func_input(shape, values, 1, nullptr, nullptr, &node);
    ::primitiv_Node_delete(node);
    ::primitiv_node_func_input(shape, values, 1, nullptr, nullptr, &node);
    ::primitiv_Node_delete(node);
    ::primitiv_Graph_num_operators(g, &num);
    EXPECT_EQ(3u, num);
  }

  ::primitiv_Graph_clear(g);
  ::primitiv_Graph_num_operators(g, &num);
  EXPECT_EQ(0u, num);

  // Clear empty graph.
  ::primitiv_Graph_clear(g);
  ::primitiv_Graph_num_operators(g, &num);
  EXPECT_EQ(0u, num);

  ::primitiv_Graph_delete(g);
}

TEST_F(CGraphTest, CheckForward) {
  ::primitiv_Status_reset();
  ::primitiv_Device_set_default(dev);

  ::primitiv_Graph *g;
  ASSERT_EQ(::primitiv_Status::PRIMITIV_OK, ::primitiv_Graph_new(&g));
  ::primitiv_Graph_set_default(g);

  const float data1[] = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
  const float data3[] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2};
  vector<::primitiv_Node*> nodes;
  const uint32_t dims1[] = {2, 2};
  ::primitiv_Shape *shape1;
  ASSERT_EQ(::primitiv_Status::PRIMITIV_OK,
            ::primitiv_Shape_new_with_dims(dims1, 2, 3, &shape1));
  ::primitiv_Node *node1;
  ::primitiv_node_func_input(shape1, data1, 12, nullptr, nullptr, &node1);
  nodes.emplace_back(node1);
  ::primitiv_Shape *shape2;
  ASSERT_EQ(::primitiv_Status::PRIMITIV_OK,
            ::primitiv_Shape_new_with_dims(dims1, 2, 1, &shape2));
  ::primitiv_Node *node2;
  ::primitiv_node_func_ones(shape2, nullptr, nullptr, &node2);
  nodes.emplace_back(node2);
  ::primitiv_Node *node3;
  ::primitiv_node_func_input(shape1, data3, 12, nullptr, nullptr, &node3);
  nodes.emplace_back(node3);
  ::primitiv_Node *node4;
  ::primitiv_node_func_add_node_node(nodes[0], nodes[1], &node4);
  nodes.emplace_back(node4);
  ::primitiv_Node *node5;
  ::primitiv_node_func_subtract_node_node(nodes[1], nodes[2], &node5);
  nodes.emplace_back(node5);
  ::primitiv_Node *node6;
  ::primitiv_node_func_multiply_node_node(nodes[3], nodes[4], &node6);
  nodes.emplace_back(node6);
  ::primitiv_Node *node7;
  ::primitiv_node_func_add_node_const(nodes[5], 1, &node7);
  nodes.emplace_back(node7);
  ::primitiv_Node *node8;
  ::primitiv_node_func_sum(nodes[6], 0, &node8);
  nodes.emplace_back(node8);
  ::primitiv_Node *node9;
  ::primitiv_node_func_sum(nodes[7], 1, &node9);
  nodes.emplace_back(node9);
  ::primitiv_Node *node10;
  ::primitiv_node_func_batch_sum(nodes[8], &node10);
  nodes.emplace_back(node10);

  EXPECT_EQ(10u, nodes.size());
  uint32_t num;
  ::primitiv_Graph_num_operators(g, &num);
  EXPECT_EQ(10u, num);

  // Dump the graph to the output log.
  std::size_t length;
  ::primitiv_Graph_dump(g, "dot", nullptr, &length);
  EXPECT_GT(length, 0u);
  char str[length];
  ::primitiv_Graph_dump(g, "dot", str, &length);
  std::cout << str;

  // Check all shapes and devices.
  const uint32_t dims2[] = {1, 2};
  ::primitiv_Shape *shape3;
  ASSERT_EQ(::primitiv_Status::PRIMITIV_OK,
            ::primitiv_Shape_new_with_dims(dims2, 2, 3, &shape3));
  const uint32_t dims3[] = {};
  ::primitiv_Shape *shape4;
  ASSERT_EQ(::primitiv_Status::PRIMITIV_OK,
            ::primitiv_Shape_new_with_dims(dims3, 0, 3, &shape4));
  ::primitiv_Shape *shape5;
  ASSERT_EQ(::primitiv_Status::PRIMITIV_OK,
            ::primitiv_Shape_new_with_dims(dims3, 0, 1, &shape5));
  const vector<::primitiv_Shape*> expected_shapes {
    shape1, shape2, shape1,
    shape1, shape1, shape1,
    shape1,
    shape3, shape4, shape5,
  };
  for (std::uint32_t i = 0; i < nodes.size(); ++i) {
    ::primitiv_Shape *shape;
    ::primitiv_Device *device;
    ::primitiv_Node_shape(nodes[i], &shape);
    ::primitiv_Node_device(nodes[i], &device);
    unsigned char eq;
    ::primitiv_Shape_op_eq(expected_shapes[i], shape, &eq);
    EXPECT_TRUE(eq);
    EXPECT_EQ(dev, device);
    ::primitiv_Shape_delete(shape);
    // ::primitiv_Device_delete(device);  // do not delete the reference
  }

  const ::primitiv_Tensor *tensor;
  ::primitiv_Graph_forward(g, nodes.back(), &tensor);
  // ::primitiv_Tensor_delete(const_cast<::primitiv_Tensor*>(tensor));
  // do not delete the reference

  // Check all node values.
  const vector<std::vector<float>> expected_values {
    {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4},
    {1, 1, 1, 1},
    {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2},
    {2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5},
    {1, 1, 1, 1, 0, 0, 0, 0, -1, -1, -1, -1},
    {2, 3, 4, 5, 0, 0, 0, 0, -2, -3, -4, -5},
    {3, 4, 5, 6, 1, 1, 1, 1, -1, -2, -3, -4},
    {7, 11, 2, 2, -3, -7},
    {18, 4, -10},
    {12},
  };
  for (std::uint32_t i = 0; i < nodes.size(); ++i) {
    // This forward method has no effect and only returns the reference to the
    // inner value.
    const ::primitiv_Tensor *val;
    ::primitiv_Graph_forward(g, nodes[i], &val);
    unsigned char valid;
    ::primitiv_Tensor_valid(val, &valid);
    ASSERT_TRUE(valid);

    std::size_t size1;
    ::primitiv_Tensor_to_array(val, nullptr, &size1);
    float array1[size1];
    ::primitiv_Tensor_to_array(val, array1, &size1);
    float *expected_array = const_cast<float*>(&(expected_values[i])[0]);
    EXPECT_TRUE(array_match(expected_array, array1, size1));

    std::size_t size2;
    ::primitiv_Node_to_array(nodes[i], nullptr, &size2);
    float array2[size2];
    ::primitiv_Node_to_array(nodes[i], array2, &size2);
    EXPECT_TRUE(array_match(expected_array, array2, size2));
  }

  ::primitiv_Shape_delete(shape1);
  ::primitiv_Shape_delete(shape2);
  ::primitiv_Shape_delete(shape3);
  ::primitiv_Shape_delete(shape4);
  ::primitiv_Shape_delete(shape5);
  for (std::uint32_t i = 0; i < nodes.size(); ++i) {
    ::primitiv_Node_delete(nodes[i]);
  }
  ::primitiv_Graph_delete(g);
}

}  // namespace c
}  // namespace primitiv

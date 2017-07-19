#ifndef PRIMITIV_GRAPH_H_
#define PRIMITIV_GRAPH_H_

#include <memory>
#include <vector>
#include <primitiv/shape.h>

namespace primitiv {

class Device;
class Function;
class Graph;
class Node;

/**
 * Pointer of a node in the computation graph.
 */
class Node {
  friend Graph;

public:
  Node(const Node &) = default;

  Node(Node &&src) : g_(src.g_), fid_(src.fid_), vid_(src.vid_) {
    src.g_ = nullptr;
  }

  Node &operator=(const Node &) = default;

  Node &operator=(Node &&src) {
    if (&src != this) {
      g_ = src.g_;
      fid_ = src.fid_;
      vid_ = src.vid_;
      src.g_ = nullptr;
    }
    return *this;
  }

  Node() : g_(nullptr), fid_(), vid_() {}

  /**
   * Returns whether the node is valid or not.
   * @return true or false w.r.t. the node is valid or not.
   */
  bool valid() const { return !!g_; }

  /**
   * Returns corresponding Graph object.
   * @return Graph object.
   */
  Graph &graph() const {
    if (!valid()) THROW_ERROR("Invalid node.");
    return *g_;
  }

  /**
   * Returns the function ID.
   * @return Function ID.
   */
  unsigned function_id() const {
    if (!valid()) THROW_ERROR("Invalid node.");
    return fid_;
  }

  /**
   * Returns the value ID of the function.
   * @return Value ID.
   */
  unsigned value_id() const {
    if (!valid()) THROW_ERROR("Invalid node.");
    return vid_;
  }

  /**
   * Returns shape of the node.
   * @return A Shape object.
   */
  const Shape &shape() const;

  /**
   * Returns device of the node.
   * @return Device object.
   */
  Device &device() const;

  /**
   * Returns the value of the node.
   * @return A Tensor object if the node has been forwarded.
   */
  const Tensor &value() const;

  /**
   * Returns the gradient of the node.
   * @return A Tensor object if the node has been backwarded.
   */
  const Tensor &gradient() const;

private:
  /**
   * Creates a new node pointer.
   * @param g Pointer of the computation graph.
   * @param fid Function ID.
   * @param vid Value ID.
   */
  Node(Graph &g, unsigned fid, unsigned vid) : g_(&g), fid_(fid), vid_(vid) {}

  Graph *g_;
  unsigned fid_;
  unsigned vid_;
};

/**
 * Computation graph.
 */
class Graph {
  Graph(const Graph &) = delete;
  Graph(Graph &&) = delete;
  Graph &operator=(const Graph &) = delete;
  Graph &operator=(Graph &&) = delete;

public:
  Graph() = default;
  ~Graph();

  /**
   * Obtains the default graph.
   * @return Reference to the default graph.
   * @throws primitiv::Error The default graph is null.
   */
  static Graph &get_default_graph();

  /**
   * Sets the default graph.
   * @param graph reference to the default graph.
   */
  static void set_default_graph(Graph &g);

  /**
   * Adds a function subgraph.
   * @param func Interface of the new function.
   * @param args List of arguments. Each node should point a node in the same
   *        computation graph.
   * @return A new Node object of the resulting value.
   */
  Node add_function(
      std::unique_ptr<Function> &&func, const std::vector<Node> &args);

  /**
   * Calculates the value of given node.
   * @param node Node object specifying the target node.
   * @return Calculated value.
   * @remarks This function calculates only the subgraph which is required to
   *          calculate the target node. Each intermediate result is stored to
   *          the corresponding node in the subgraph and they are re-used for
   *          future calculation. I.e., each node is calculated only once while
   *          the lifetime of the Graph object.
   */
  const Tensor &forward(const Node &node);

  /**
   * Calculates the backpropagation.
   * @param node Node object specifying the output node.
   * @remarks `node` should point to a node in the forward path, i.e., the same
   *          node used to call `forward()`, or an ancestor node of that.
   *          Descendant nodes of `node` are removed from the backward path.
   */
  void backward(const Node &node);

  /**
   * Retrieves the shape of the node.
   * @param node Node object specifying the target node.
   * @return The shape of the node.
   */
  const Shape &get_shape(const Node &node) const;

  /**
   * Retrieves the device of the node.
   * @param node Node object specifying the target node.
   * @return Device of the node.
   */
  Device &get_device(const Node &node) const;

  /**
   * Retrieves the value of the node.
   * @param node Node object specifying the target node.
   * @return Calculated value if it is already calculated, or an invalid tensor
   *         otherwise.
   * @remarks This method does not affect the internal information of the graph.
   */
  const Tensor &get_value(const Node &node) const;

  /**
   * Retrieves the gradient of the node.
   * @param node Node object specifying the target node.
   * @return Calculated value if it is already calculated, or an invalid tensor
   *         otherwise.
   * @remarks This method does not affect the internal information of the graph.
   */
  const Tensor &get_gradient(const Node &node) const;

  /**
   * Dump internal graphs.
   */
  void dump() const;

  /**
   * Returns the number of functions in the computation graph.
   * @return Number of nodes.
   */
  unsigned num_functions() const { return funcs_.size(); }

private:
  /**
   * Tuple of values to determine the location of the node.
   */
  struct Address {
    unsigned fid;
    unsigned vid;
  };

  /**
   * Informations of each node.
   */
  struct NodeInfo {
    Shape shape;
    Device &device;
    std::unique_ptr<Tensor> value;
    std::unique_ptr<Tensor> grad;
    std::vector<unsigned> sinks;
  };

  /**
   * Set of informations that represents the function: an implementation of the
   * function, its arguments, and its return values.
   */
  struct FunctionInfo {
    std::unique_ptr<Function> func;
    std::vector<Address> args;
    std::vector<NodeInfo> rets;
  };

  static Graph *default_graph_;

  std::vector<FunctionInfo> funcs_;
};

inline const Shape &Node::shape() const {
  if (!valid()) THROW_ERROR("Invalid node.");
  return g_->get_shape(*this);
}

inline Device &Node::device() const {
  if (!valid()) THROW_ERROR("Invalid node.");
  return g_->get_device(*this);
}

inline const Tensor &Node::value() const {
  if (!valid()) THROW_ERROR("Invalid node.");
  return g_->get_value(*this);
}

inline const Tensor &Node::gradient() const {
  if (!valid()) THROW_ERROR("Invalid node.");
  return g_->get_gradient(*this);
}

}  // namespace primitiv

#endif  // PRIMITIV_GRAPH_H_

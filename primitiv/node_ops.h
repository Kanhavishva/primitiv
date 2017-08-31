#ifndef PRIMITIV_NODE_OPS_H_
#define PRIMITIV_NODE_OPS_H_

#include <vector>
#include <primitiv/device.h>
#include <primitiv/graph.h>

namespace primitiv {

class Node;
class Parameter;

namespace operators {

Node sum(const std::vector<Node> &xs);
Node sum_ptr(const std::vector<const Node *> &xs);
Node mean(const Node &x, unsigned dim);
Node mean(const std::vector<Node> &xs);
Node mean_ptr(const std::vector<const Node *> &xs);

Node dropout(const Node &x, float rate, bool enabled);

namespace batch {
Node mean(const Node &x);
Node normalize(const Node &x);
}  // namespace batch

Node zeros(
    const Shape &shape,
    Device &dev = Device::get_default_device(),
    Graph &g = Graph::get_default_graph());
Node ones(
    const Shape &shape,
    Device &dev = Device::get_default_device(),
    Graph &g = Graph::get_default_graph());
Node constant(
    const Shape &shape, float k,
    Device &dev = Device::get_default_device(),
    Graph &g = Graph::get_default_graph());

namespace random {
Node bernoulli(
    const Shape &shape, float p,
    Device &dev = Device::get_default_device(),
    Graph &g = Graph::get_default_graph());
Node uniform(
    const Shape &shape, float lower, float upper,
    Device &dev = Device::get_default_device(),
    Graph &g = Graph::get_default_graph());
Node normal(
    const Shape &shape, float mean, float sd,
    Device &dev = Device::get_default_device(),
    Graph &g = Graph::get_default_graph());
Node log_normal(
    const Shape &shape, float mean, float sd,
    Device &dev = Device::get_default_device(),
    Graph &g = Graph::get_default_graph());
} // namespace random

}  // namespace operators

}  // namespace primitiv

#endif  // PRIMITIV_NODE_OPS_H_

#include "node.hpp"

Node::Node(long int number, double r, std::vector<double> temperatures, Precondition precondition) : number(number),
                                                                                                     r(r),
                                                                                                     temperatures(temperatures),
                                                                                                     precondition(precondition)
{

}

std::ostream& operator<<(std::ostream& out, const Node& node)
{
    return out << "Node nr " << node.number << ", r: " << node.r << ", Temperature: " << node.temperatures.at(0) << ", PRECONDITION: " << node.precondition << std::endl;
}

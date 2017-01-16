#include "node.hpp"

Node::Node(long int number, Precondition precondition) : number(number), precondition(precondition)
{

}

std::ostream& operator<<(std::ostream& out, const Node& node)
{
    return out << "Node nr " << node.number << ", PRECONDITION: " << node.precondition << std::endl;
}

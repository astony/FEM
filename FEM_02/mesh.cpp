#include <QDebug>
#include "mesh.hpp"

Mesh::Mesh(std::map<int, Node*> nodes_set, std::map<int, Element*> elements_set) : nodes_set(nodes_set), elements_set(elements_set)
{

}

std::ostream& operator <<(std::ostream& out, const Mesh& mesh)
{
    for(auto i : mesh.elements_set)
        out << *i.second << std::endl;
    out << "------------------------------" << std::endl;
    return out;
}

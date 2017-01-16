#ifndef MESH_HPP
#define MESH_HPP

#include <QVector>
#include <map>
#include <element.hpp>

class Mesh
{
public:
    Mesh(std::map<int, Node*>, std::map<int, Element*>);
    std::map<int, Node*> nodes_set;
    std::map<int, Element*> elements_set;
    friend std::ostream& operator <<(std::ostream&, const Mesh&);
};

std::ostream& operator <<(std::ostream&, const Mesh&);

#endif // MESH_HPP

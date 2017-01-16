#ifndef NODE_HPP
#define NODE_HPP

#include <QObject>
#include <QDebug>

class Node : public QObject
{
    Q_OBJECT
public:
    enum Precondition { NONE, CONVECTION, RADIATION };
    Node(long int, Precondition = Precondition::NONE);
    Q_ENUM(Precondition)
    friend std::ostream& operator<<(std::ostream&, const Node&);
    long int number;
    Precondition precondition;
};

std::ostream& operator<<(std::ostream&, const Node&);

#endif // NODE_HPP

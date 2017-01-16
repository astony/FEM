#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <QDebug>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <node.hpp>

class Element
{
public:
    Element(long int, Node*, Node*, double, double, double, double, double, double);
    ~Element();
    friend std::ostream& operator<<(std::ostream&, const Element&);
    double C() const;
    // Matrix [2, 2]
    boost::numeric::ublas::matrix<double>* H();
    // Vector [2, 1]
    boost::numeric::ublas::matrix<double>* P();

    long int number;
    Node* left_node;
    Node* right_node;

private:
    int N_left, N_right;
    double S, k, L, AT, alfa, q;
};

std::ostream& operator<<(std::ostream&, const Element&);

#endif // ELEMENT_HPP

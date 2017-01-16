#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <QDebug>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <node.hpp>

class Element
{
public:
    Element(long int, Node*, Node*, double);
    ~Element();
    friend std::ostream& operator<<(std::ostream&, const Element&);
    // Matrix [2, 2]
    boost::numeric::ublas::matrix<double>* K();
    // Vector [2, 1]
    boost::numeric::ublas::matrix<double>* F();

    long int number;
    Node* left_node;
    Node* right_node;
    double alfa;

    static std::vector<double> gauss_points;
    static std::vector<double> gauss_weights;

    static double c;
    static double ro;
    static double tau;
    static double delta_tau;
    static double r_max;
    static double t0;
    static double too;
    static double k;
    static double max_temperature;

private:
    double gauss_R(double, double, double);
    double N(int, double);
};

std::ostream& operator<<(std::ostream&, const Element&);

#endif // ELEMENT_HPP

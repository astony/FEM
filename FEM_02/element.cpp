#include "element.hpp"

std::vector<double> Element::gauss_points{-0.5773502692f, 0.5773502692f};
std::vector<double> Element::gauss_weights{1.0f, 1.0f};

double Element::c = 700;
double Element::ro = 7800;
double Element::tau = 0;
double Element::delta_tau = 50;
double Element::r_max = 0.08;
double Element::t0 = 100;
double Element::too = 200;
double Element::k = 25;
double Element::max_temperature = 1000;

Element::Element(long int number, Node* left_node, Node* right_node, double alfa) : number(number),
                                                                                    left_node(left_node),
                                                                                    right_node(right_node),
                                                                                    alfa(alfa)
{

}

std::ostream& operator<<(std::ostream& out, const Element& element)
{
    return out << "Element nr " << element.number << ")" << std::endl << *element.left_node << *element.right_node << std::endl;
}

boost::numeric::ublas::matrix<double>* Element::K()
{
    boost::numeric::ublas::matrix<double> *K = new boost::numeric::ublas::matrix<double>(2,2);

    double br1{}, br2{};

    if (left_node->precondition != Node::Precondition::NONE)
        br1 = 2 * alfa * r_max;

    if (right_node->precondition != Node::Precondition::NONE)
        br2 = 2 * alfa * r_max;


    double ri = left_node->r;
    double rj = right_node->r;

    double delta_r = rj - ri;

    double k11{}, k12{}, k21{}, k22{};

    for (unsigned long i = 0; i < gauss_points.size(); ++i)
    {
        k11 += ((k / delta_r) * gauss_R(gauss_points[i], ri, rj) * gauss_weights[i]) + ((c * ro * delta_r) / delta_tau) * pow(N(1, gauss_points[i]), 2.0) * gauss_R(gauss_points[i], ri, rj) * gauss_weights[i];

        k12 += -((k / delta_r) * gauss_R(gauss_points[i], ri, rj) * gauss_weights[i]) + (((c * ro * delta_r) / delta_tau) * (N(1, gauss_points[i])) * (N(2, gauss_points[i])) * gauss_R(gauss_points[i], ri, rj) * gauss_weights[i]);

        k22 += ((k / delta_r) * gauss_R(gauss_points[i], ri, rj) * gauss_weights[i]) + ((c * ro * delta_r) / delta_tau) * pow(N(2, gauss_points[i]), 2.0) * gauss_R(gauss_points[i], ri, rj) * gauss_weights[i];
    }

    k11 += br1;
    k22 += br2;

    k21 = k12;

    K->insert_element(0, 0, k11);
    K->insert_element(0, 1, k12);
    K->insert_element(1, 0, k21);
    K->insert_element(1, 1, k22);

    return K;
}

boost::numeric::ublas::matrix<double>* Element::F()
{
    boost::numeric::ublas::matrix<double> *F = new boost::numeric::ublas::matrix<double>(2,1);

    double br1{}, br2{};

    if (left_node->precondition != Node::Precondition::NONE)
        br1 = -2 * alfa * r_max * too;

    if (right_node->precondition != Node::Precondition::NONE)
        br2 = -2 * alfa * r_max * too;

    double ri = left_node->r;
    double rj = right_node->r;

    double delta_r = rj - ri;

    double k11{}, k21{};

    for (unsigned long i = 0; i < gauss_points.size(); ++i)
    {
        k11 += -((c * ro * delta_r) / delta_tau) * (N(1, gauss_points[i]) * left_node->temperatures.back() + N(2, gauss_points[i]) * right_node->temperatures.back()) * N(1, gauss_points[i]) * gauss_R(gauss_points[i], ri, rj) * gauss_weights[i];

        k21 += -((c * ro * delta_r) / delta_tau) * (N(1, gauss_points[i]) * left_node->temperatures.back() + N(2, gauss_points[i]) * right_node->temperatures.back()) * N(2, gauss_points[i]) * gauss_R(gauss_points[i], ri, rj) * gauss_weights[i];
    }

    k11 += br1;
    k21 += br2;

    F->insert_element(0, 0, k11);
    F->insert_element(1, 0, k21);

    return F;
}

double Element::gauss_R(double E, double i, double j)
{
    return 0.5 * (1 - E) * i + 0.5 * (1 + E) * j;
}

double Element::N(int n, double E) {
    if (n == 1)
        return 0.5 * (1 - E);
    else if (n == 2)
        return 0.5 * (1 + E);
    else
        return 0.0;
}

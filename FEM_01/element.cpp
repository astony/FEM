#include "element.hpp"

Element::Element(long int number, Node* left_node, Node* right_node, double S, double k, double L,
                 double AT, double alfa, double q) : number(number),
                                                     left_node(left_node),
                                                     right_node(right_node),
                                                     S(S),
                                                     k(k),
                                                     L(L),
                                                     AT(AT),
                                                     alfa(alfa),
                                                     q(q)
{

}

double Element::C() const
{
    return (S*k)/L;
}

std::ostream& operator<<(std::ostream& out, const Element& element)
{
    return out << "Element nr " << element.number << ")" << std::endl << *element.left_node << *element.right_node << "C: " << element.C() << std::endl;
}

boost::numeric::ublas::matrix<double>* Element::H()
{
    boost::numeric::ublas::matrix<double> *H = new boost::numeric::ublas::matrix<double>(2,2);

    // Preconditions only CONVECTION
    if(left_node->precondition == 1)
    {
        N_left = 1; N_right = 0;
    }
    else if(right_node->precondition == 1)
    {
        N_left = 0; N_right = 1;
    }
    else
    {
        N_left = 0; N_right = 0;
    }

    for(unsigned int i = 0; i < H->size1(); ++i)
        for(unsigned int j = 0; j < H->size2(); ++j)
        {
            if(i == 0 && j == 0)
                H->insert_element(i, j, C() + N_left * N_left * alfa * S);
            else if(i == 1 && j == 1)
                H->insert_element(i, j, C() + N_right * N_right * alfa * S);
            else
                H->insert_element(i, j, -C() + N_left * N_right * alfa * S);
        }
    return H;
}

boost::numeric::ublas::matrix<double>* Element::P()
{
    boost::numeric::ublas::matrix<double> *P = new boost::numeric::ublas::matrix<double>(2,1);

    // Preconditions both CONVECTION and RADIATION
    if(left_node->precondition != 0)
    {
        N_left = 1; N_right = 0;
    }
    else if(right_node->precondition != 0)
    {
        N_left = 0; N_right = 1;
    }
    else
    {
        N_left = 0; N_right = 0;
    }

    double precondition;
    if(left_node->precondition == Node::Precondition::RADIATION || right_node->precondition == Node::Precondition::RADIATION)
        precondition = q * S;
    if(left_node->precondition == Node::Precondition::CONVECTION || right_node->precondition == Node::Precondition::CONVECTION)
        precondition = -alfa * AT * S;

    for(unsigned int i = 0; i < P->size1(); ++i)
        for(unsigned int j = 0; j < P->size2(); ++j)
            if(i == 0)
                P->insert_element(i, j, N_left * precondition);
            else if (i == 1)
                P->insert_element(i, j, N_right * precondition);
    return P;
}

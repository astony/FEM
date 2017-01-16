#include <QCoreApplication>

#include "algorithm_method.hpp"
#include <boost/numeric/ublas/io.hpp>

int main()
{
    std::cout << "3 Nodes:" << std::endl;
    std::cout << "------------------------------" << std::endl << std::endl;
    AlgorithmMethod mes_3("data/mesh_3.txt");
    mes_3.GT();

    std::cout << "5 Nodes:" << std::endl;
    std::cout << "------------------------------" << std::endl << std::endl;
    AlgorithmMethod mes_5("data/mesh_5.txt");
    mes_5.GT();

    return 0;
}

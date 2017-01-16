#ifndef ALGORITHMMETHOD_HPP
#define ALGORITHMMETHOD_HPP

#include <QVector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <mesh.hpp>

class AlgorithmMethod
{
public:
    AlgorithmMethod(QString);
    Mesh* mesh;
    boost::numeric::ublas::matrix<double>* GT();
private:
    boost::numeric::ublas::matrix<double>* GH();
    boost::numeric::ublas::matrix<double>* GP();
    Mesh* readMeshFromFile(QString);
    template <class T>
    bool gauss_solve(boost::numeric::ublas::matrix<T>&, boost::numeric::ublas::matrix<T>&);
};

#endif // ALGORITHMMETHOD_HPP

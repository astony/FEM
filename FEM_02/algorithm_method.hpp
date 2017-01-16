#ifndef ALGORITHMMETHOD_HPP
#define ALGORITHMMETHOD_HPP

#include <QVector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <mesh.hpp>

class AlgorithmMethod
{
public:
    AlgorithmMethod();
    Mesh* mesh;
    void solve();
private:
    void assign_temperatures(boost::numeric::ublas::matrix<double>*, Mesh*);
    boost::numeric::ublas::matrix<double>* GK();
    boost::numeric::ublas::matrix<double>* GF();
    boost::numeric::ublas::matrix<double>* GT();
    Mesh* read_mesh_from_file(QString);
    template <class T>
    bool gauss_matrix(boost::numeric::ublas::matrix<T>&, boost::numeric::ublas::matrix<T>&);
};

#endif // ALGORITHMMETHOD_HPP

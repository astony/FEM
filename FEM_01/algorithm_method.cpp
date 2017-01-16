#include <QFile>
#include <QTextStream>
#include <QString>
#include <QMetaEnum>
#include <QMetaObject>
#include "algorithm_method.hpp"

AlgorithmMethod::AlgorithmMethod(QString fileName)
{
    mesh = readMeshFromFile(fileName);
    std::cout << *mesh << std::endl;
}

boost::numeric::ublas::matrix<double>* AlgorithmMethod::GH()
{
    unsigned long nodes_size = mesh->nodes_set.size();
    boost::numeric::ublas::matrix<double>* GH = new boost::numeric::ublas::matrix<double>(nodes_size, nodes_size);

    for(unsigned long j = 0; j < GH->size1(); ++j)
        for(unsigned long k = 0; k < GH->size2(); ++k)
            GH->insert_element(j, k, 0.0f);

    for(auto i : mesh->elements_set)
    {
        Element* element = i.second;
        boost::numeric::ublas::matrix<double>* H = element->H();
        std::cout << "H for element nr " << element->number << std::endl << *H << std::endl << std::endl;

        for(unsigned long j = 0; j < H->size1(); ++j)
            for(unsigned long k = 0; k < H->size2(); ++k)
            {
                if(j < k)
                    GH->at_element(element->left_node->number - 1, element->right_node->number - 1) += H->at_element(j, k);
                else if (j > k)
                    GH->at_element(element->right_node->number - 1, element->left_node->number - 1) += H->at_element(j, k);
                else if (j == 0 && k == 0)
                    GH->at_element(element->left_node->number - 1, element->left_node->number - 1) += H->at_element(j, k);
                else if (j == 1 && k == 1)
                    GH->at_element(element->right_node->number - 1, element->right_node->number - 1) += H->at_element(j, k);
            }
    }
    std::cout << "GH: " << std::endl << *GH << std::endl << std::endl;
    return GH;
}

boost::numeric::ublas::matrix<double>* AlgorithmMethod::GP()
{
    unsigned long nodes_size = mesh->nodes_set.size();
    boost::numeric::ublas::matrix<double>* GP = new boost::numeric::ublas::matrix<double>(nodes_size, 1);

    for(unsigned long j = 0; j < GP->size1(); ++j)
        for(unsigned long k = 0; k < GP->size2(); ++k)
            GP->insert_element(j, k, 0.0f);

    for(auto i : mesh->elements_set)
    {
        Element* element = i.second;
        boost::numeric::ublas::matrix<double>* P = element->P();
        std::cout << "P for element nr " << element->number << std::endl << *P << std::endl << std::endl;

        for(unsigned long j = 0; j < P->size1(); ++j)
            for(unsigned long k = 0; k < P->size2(); ++k)
                if(j == 0)
                    GP->at_element(element->left_node->number - 1, k) += P->at_element(j, k);
                else if (j == 1)
                    GP->at_element(element->right_node->number - 1, k) += P->at_element(j, k);

    }
    std::cout << "GP: " << std::endl << *GP << std::endl << std::endl;
    return GP;
}

/* Result after solving system of linear equations
 *
 * [GH]*{GT} + {GP} = 0
 * [GH]*{GT} = -{GP}
 *
 */
boost::numeric::ublas::matrix<double>* AlgorithmMethod::GT()
{
    boost::numeric::ublas::matrix<double>* GH = this->GH();
    boost::numeric::ublas::matrix<double>* GP = this->GP();

    for(unsigned long j = 0; j < GP->size1(); ++j)
        for(unsigned long k = 0; k < GP->size2(); ++k)
            GP->insert_element(j, k, -GP->at_element(j, k));

    bool solved = gauss_solve(*GH, *GP);

    if(solved == true)
    {
        std::cout << "Solved: true" << std::endl;
        std::cout << "GT: " << std::endl << *GP << std::endl << std::endl;
    }
    else
        std::cout << "Solved: false" << std::endl;

    return GP;
}

/* works only for 1D
 * 1 - number of nodes
 * 2 - node number, NONE | CONVECTION | RADIATION
 * .
 * .
 * .
 * (number of nodes + 1)
 * 3 - element number, left node number, right node number, S, k, L, ambient temperature, alfa, q
 * .
 * .
 * .
 * (2 * number of nodes)
 *
 */
Mesh* AlgorithmMethod::readMeshFromFile(QString fileName)
{
    std::map<int, Node*> nodes_set;
    std::map<int, Element*> elements_set;

    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in(&file);
    in.setPadChar(',');
    QString helpString;
    QStringList helpStringList;

    helpString = in.readLine();

    long int nodes_count(helpString.toLong());

    long int number;
    Node::Precondition precondition;

    for(long int i = 0; i < nodes_count; ++i)
    {
        helpString = in.readLine();
        helpStringList = helpString.split(',');

        number = helpStringList[0].toLong();

        const QMetaObject &mo = Node::staticMetaObject;

        int index = mo.indexOfEnumerator("Precondition");
        QMetaEnum metaEnum = mo.enumerator(index);
        int value = metaEnum.keyToValue(qPrintable(helpStringList[1]));

        precondition = static_cast<Node::Precondition>(value);

        Node* node = new Node(number, precondition);
        nodes_set.insert({number, node});
    }

    long int left_node_number;
    long int right_node_number;
    double S, k, L;
    double AT, alfa, q;

    for(long int i = 0; i < nodes_count - 1; ++i)
    {
        helpString = in.readLine();
        helpStringList = helpString.split(',');

        number = helpStringList[0].toLong();
        left_node_number = helpStringList[1].toLong();
        right_node_number = helpStringList[2].toLong();
        S = helpStringList[3].toDouble();
        k = helpStringList[4].toDouble();
        L = helpStringList[5].toDouble();
        AT = helpStringList[6].toDouble();
        alfa = helpStringList[7].toDouble();
        q = helpStringList[8].toDouble();

        auto left_node = nodes_set.find(left_node_number)->second;
        auto right_node = nodes_set.find(right_node_number)->second;
        Element* element = new Element(number, left_node, right_node, S, k, L, AT, alfa, q);
        elements_set.insert({number, element});
    }

    return new Mesh(nodes_set, elements_set);
}

/*
 *  Solves a linear system A x X = B using Gauss elimination,
 *  given by Boost uBlas matrices 'a' and 'b'.
 *
 *  If the elimination succeeds, the function returns true,
 *  A is inverted and B contains values for X.
 *
 *  In case A is singular (linear system is unsolvable), the
 *  function returns false and leaves A and B in scrambled state.
 *
 *  TODO: make further use of uBlas views instead of direct
 *  element access (for better optimizations)
 */
template <class T>
bool AlgorithmMethod::gauss_solve(boost::numeric::ublas::matrix<T>& a, boost::numeric::ublas::matrix<T>& b)
{
 int icol, irow;
 int n = a.size1();
 int m = b.size2();

 int* indxc = new int[n];
 int* indxr = new int[n];
 int* ipiv = new int[n];

 typedef boost::numeric::ublas::matrix<T> GJ_Mtx;
 typedef boost::numeric::ublas::matrix_row<GJ_Mtx> GJ_Mtx_Row;
 typedef boost::numeric::ublas::matrix_column<GJ_Mtx> GJ_Mtx_Col;

 for (int j=0; j<n; ++j)
   ipiv[j]=0;

 for (int i=0; i<n; ++i)
 {
   T big=0.0;
   for (int j=0; j<n; j++)
     if (ipiv[j] != 1)
       for (int k=0; k<n; k++)
       {
         if (ipiv[k] == 0)
         {
           T cmpa = a(j,k);
           if ( cmpa < 0)
             cmpa = -cmpa;
           if (cmpa >= big)
           {
             big = cmpa;
             irow=j;
             icol=k;
           }
         }
         else if (ipiv[k] > 1)
           return false;
       }

   ++(ipiv[icol]);
   if (irow != icol)
   {
     GJ_Mtx_Row ar1(a, irow), ar2 (a, icol);
     ar1.swap(ar2);

     GJ_Mtx_Row br1(b, irow), br2(b, icol);
     br1.swap(br2);
   }

   indxr[i] = irow;
   indxc[i] = icol;
   if (a(icol, icol) == 0.0)
     return false;

   T pivinv = 1.0 / a(icol, icol);
   a(icol, icol) = 1.0;
   GJ_Mtx_Row(a, icol) *= pivinv;
   GJ_Mtx_Row(b, icol) *= pivinv;

   for (int ll=0; ll<n; ll++)
     if (ll != icol)
     {
       T dum = a(ll, icol);
       a(ll, icol) = 0.0;
       for (int l=0; l<n; l++)
         a(ll, l) -= a(icol, l) * dum;
       for (int l=0; l<m; l++)
         b(ll, l) -= b(icol, l) * dum;
     }
 }

 // Unscramble A's columns
 for (int l=n-1; l>=0; --l)
   if (indxr[l] != indxc[l])
   {
     GJ_Mtx_Col ac1(a, indxr[l]), ac2(a, indxc[l]);
     ac1.swap(ac2);
   }

 delete[] ipiv;
 delete[] indxr;
 delete[] indxc;

 return true;
}



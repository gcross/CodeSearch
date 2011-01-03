//@+leo-ver=5-thin
//@+node:gcross.20101229110857.1618: * @thin standard_form.hpp
//@@language cplusplus

#ifndef CONSTRAINTS_STANDARD_FORM_HPP
#define CONSTRAINTS_STANDARD_FORM_HPP

//@+<< Includes >>
//@+node:gcross.20101229110857.1619: ** << Includes >>
#include "operator_space.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101229110857.1620: ** << Usings >>
using namespace CodeQuest;
using namespace Gecode;
using namespace boost;
using namespace std;
//@-<< Usings >>

//@+others
//@+node:gcross.20110102182304.1571: ** Classes
//@+node:gcross.20101231214817.2153: *3* StandardFormParameters
struct StandardFormParameters {
    unsigned int x_bit_diagonal_size, z_bit_diagonal_size;
    StandardFormParameters(const unsigned int x_bit_diagonal_size, const unsigned int z_bit_diagonal_size);
    bool operator==(const StandardFormParameters& other) const;
};
//@+node:gcross.20101231214817.2272: *3* StandardFormIterator
class StandardFormIterator
    : public iterator_facade
            <   StandardFormIterator
            ,   StandardFormParameters const
            ,   single_pass_traversal_tag
> {

    //@+others
    //@+node:gcross.20101231214817.2273: *4* (fields)
    protected:
        const unsigned int maximum_diagonal_size;
        StandardFormParameters parameters;
    //@+node:gcross.20101231214817.2274: *4* (constructors)
    public:
        StandardFormIterator(
              const unsigned int number_of_qubits
            , const unsigned int number_of_operators
            , const bool end=false
        );
    //@+node:gcross.20101231214817.2275: *4* (methods)
    public:
        const StandardFormParameters& dereference() const;
        bool equal(const StandardFormIterator& other) const;
        void increment();
    //@-others

};
//@+node:gcross.20101229110857.1637: ** Functions
iterator_range<StandardFormIterator> generateStandardFormsFor
    ( const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    );


void postStandardFormConstraint(
      OperatorSpace& space
    , const unsigned int x_bit_diagonal_size
    , const unsigned int z_bit_diagonal_size
);

void postStandardFormConstraint(OperatorSpace& space, const StandardFormParameters& parameters);
//@-others

}

#endif
//@-leo

//@+leo-ver=5-thin
//@+node:gcross.20101229110857.1618: * @thin standard_form.hpp
//@@language cplusplus

#ifndef STANDARD_FORM_HPP
#define STANDARD_FORM_HPP

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
//@+node:gcross.20101229110857.1637: ** Functions
void postStandardFormConstraint(OperatorSpace& space, const unsigned int x_bit_diagonal_size, const unsigned int z_bit_diagonal_size);
//@-others

}

#endif
//@-leo

//@+leo-ver=5-thin
//@+node:gcross.20101224191604.2768: * @thin utilities.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101224191604.2769: ** << Includes >>
#include "solution_iterator.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

using namespace Gecode;

//@+others
//@+node:gcross.20101224191604.2770: ** Functions
//@+node:gcross.20101224191604.2772: *3* countSolutions
unsigned long long countSolutions(auto_ptr<OperatorSpace> initial_space, const Search::Options& options) {
    long long number_of_solutions = 0u;
    BOOST_FOREACH(const OperatorSpace& space, generateSolutionsFor(initial_space)) { ++number_of_solutions; }
    return number_of_solutions;
}
//@-others

}
//@-leo

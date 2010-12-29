//@+leo-ver=5-thin
//@+node:gcross.20101224191604.2748: * @thin test_utils.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101224191604.2749: ** << Includes >>
#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/irange.hpp>
#include <codequest.hpp>
#include <functional>
#include <illuminate.hpp>

#include "solution_iterator.hpp"
#include "utilities.hpp"
#include "test_utils.hpp"

using namespace CodeSearch;
using namespace Gecode;
using namespace boost;
using namespace boost::adaptors;
using namespace boost::algorithm;
using namespace std;
//@-<< Includes >>

//@+others
//@+node:gcross.20101229110857.1592: ** Variables
CodeTable operator_space_code_table;
//@+node:gcross.20101224191604.2750: ** Functions
//@+node:gcross.20101224191604.4005: *3* encodeOperatorSpace
long long encodeOperatorSpace(const OperatorSpace& space) {
    unsigned long long solution = 0u;
    BOOST_FOREACH(unsigned int i, irange(0u,space.number_of_variables)) {
        solution <<= 2;
        solution += space.O[i].val();
    }
    return solution;
}
//@+node:gcross.20101224191604.2754: *3* gatherSolutions
vector<unsigned long long> gatherSolutions(auto_ptr<OperatorSpace> initial_space) {
    vector<unsigned long long> solutions;
    copy(
        generateSolutionsFor(initial_space) | transformed(encodeOperatorSpace),
        back_inserter(solutions)
    );
}
//@-others
//@-leo

//@+leo-ver=5-thin
//@+node:gcross.20101224191604.3363: * @thin solution_iterator.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101224191604.3364: ** << Includes >>
#include "solution_iterator.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101224191604.3365: ** << Usings >>
using namespace Gecode;
using namespace boost;
using namespace std;
//@-<< Usings >>

//@+others
//@+node:gcross.20101224191604.3403: ** class SolutionIterator
//@+node:gcross.20101224191604.4180: *3* (constructors)
SolutionIterator::SolutionIterator() { }

SolutionIterator::SolutionIterator
(   auto_ptr<OperatorSpace> initial_space
,   const Search::Options& options
) {
    shared_ptr<DFS<OperatorSpace> > dfs(new DFS<OperatorSpace>(initial_space.get()));
    auto_ptr<OperatorSpace> space(dfs->next());
    if(NULL != space.get()) {
        solution_number = 0;
        this->space = space;
        this->next = wrapSharedPtr(new pair<unsigned long long,shared_ptr<DFS<OperatorSpace> > >(0,dfs));
    }
}
//@+node:gcross.20101224191604.3407: *3* dereference
const OperatorSpace& SolutionIterator::dereference() const throw() {
    return *space;
}
//@+node:gcross.20101224191604.3405: *3* equal
bool SolutionIterator::equal(const SolutionIterator& other) const throw() {
    return solution_number == other.solution_number;
}
//@+node:gcross.20101224191604.3409: *3* increment
void SolutionIterator::increment() throw() {
    auto_ptr<OperatorSpace> space(next->second->next());
    if(NULL == space.get()) {
        solution_number = none;
        space.reset();
        next.reset();
    } else {
        solution_number = next->first++;
        this->space = space;
    }
}
//@+node:gcross.20101227130707.1452: ** Functions
//@+node:gcross.20101227130707.1453: *3* generateSolutionsFor
iterator_range<SolutionIterator> generateSolutionsFor
(   auto_ptr<OperatorSpace> initial_space
,   const Search::Options& options
) {
    return iterator_range<SolutionIterator>(SolutionIterator(initial_space,options),SolutionIterator());
}
//@-others

}
//@-leo

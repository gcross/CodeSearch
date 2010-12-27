//@+leo-ver=5-thin
//@+node:gcross.20101224191604.2705: * @thin solution_iterator.hpp
//@@language cplusplus

#ifndef SOLUTION_ITERATOR_HPP
#define SOLUTION_ITERATOR_HPP

//@+<< Includes >>
//@+node:gcross.20101224191604.2706: ** << Includes >>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <codequest.hpp>
#include <memory>
#include <vector>

#include "operator_space.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101224191604.2707: ** << Usings >>
using namespace Gecode;
using namespace boost;
using namespace std;
//@-<< Usings >>

//@+others
//@+node:gcross.20101224191604.2709: ** class SolutionIterator
class SolutionIterator
    : public iterator_facade
            <   SolutionIterator
            ,   OperatorSpace const
            ,   single_pass_traversal_tag
> {

    //@+others
    //@+node:gcross.20101224191604.2711: *3* (fields)
    protected:
        optional<unsigned long long> solution_number;
        shared_ptr<OperatorSpace> space;
        shared_ptr<pair<unsigned long long,shared_ptr<DFS<OperatorSpace> > > > next;
    //@+node:gcross.20101224191604.2712: *3* (constructors)
    public:
        SolutionIterator();
        SolutionIterator(auto_ptr<OperatorSpace> initial_space, const Search::Options& options = Search::Options::def);
    //@+node:gcross.20101224191604.2714: *3* (methods)
    public:
        const OperatorSpace& dereference() const throw();
        bool equal(const SolutionIterator& other) const throw();
        void increment() throw();
    //@-others

};
//@+node:gcross.20101227130707.1449: ** Functions
iterator_range<SolutionIterator> generateSolutionsFor
    ( auto_ptr<OperatorSpace> initial_space
    , const Search::Options& options = Search::Options::def
    );
//@-others

}

#endif
//@-leo

//@+leo-ver=5-thin
//@+node:gcross.20101224191604.2760: * @thin utilities.hpp
//@@language cplusplus

#ifndef UTILITIES_HPP
#define UTILITIES_HPP

//@+<< Includes >>
//@+node:gcross.20101224191604.2761: ** << Includes >>
#include <boost/shared_ptr.hpp>
#include <gecode/set.hh>
#include <memory>
#include <utility>
#include <vector>

#include "operator_space.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101224191604.4183: ** << Usings >>
using namespace Gecode;
using namespace boost;
using namespace std;
//@-<< Usings >>

//@+others
//@+node:gcross.20101224191604.2762: ** Functions
IntMatrix channelMatrix(Space& space,const BoolMatrix& matrix);

unsigned long long countSolutions(auto_ptr<OperatorSpace> space, const Search::Options& options = Search::Options::def);

template<typename T> auto_ptr<T> wrapAutoPtr(T* ptr) { return auto_ptr<T>(ptr); }
template<typename T> shared_ptr<T> wrapSharedPtr(T* ptr) { return shared_ptr<T>(ptr); }
//@-others

}

#endif
//@-leo

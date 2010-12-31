//@+leo-ver=5-thin
//@+node:gcross.20101229110857.1676: * @thin column_ordering.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.1677: ** << Includes >>
#include <boost/foreach.hpp>

#include "constraints.hpp"
#include "operator_space.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101229110857.1678: ** << Usings >>
using namespace Gecode;
//@-<< Usings >>

//@+others
//@+node:gcross.20101229110857.2242: ** class ColumnOrderingConstraint
class ColumnOrderingConstraint : public OperatorSpace::AuxiliaryData {
//@+others
//@+node:gcross.20101229110857.2243: *3* (fields)
private:
    const unsigned int number_of_ties;
    BoolVarArray ties;
//@+node:gcross.20101229110857.2245: *3* (constructors)
public:

ColumnOrderingConstraint(OperatorSpace& space, const unsigned int start, const unsigned int end)
    : number_of_ties(end-start-1)
    , ties(space,(space.number_of_operators-1)*number_of_ties,0,1)
{
    assert(end-start >= 2 && "column ordering constraints must involve at least two columns");
    if(number_of_ties <= 0) return;

    const unsigned int number_of_operators = space.number_of_operators;

    IntMatrix O_matrix = space.getOMatrix().slice(start,end,0,number_of_operators);

    rel(space,O_matrix.row(0),IRT_GQ);

    BoolMatrix ties_matrix(ties,number_of_ties,number_of_operators-1);

    BOOST_FOREACH(const unsigned int row, irange(0u,number_of_operators-1)) {
        BOOST_FOREACH(const unsigned int col, irange(0u,number_of_ties)) {
            if(row == 0) {
                ties_matrix(col,row) = expr(space,O_matrix(col,row) == O_matrix(col+1,row));
            } else {
                ties_matrix(col,row) = expr(space,O_matrix(col,row) == O_matrix(col+1,row) && ties_matrix(col,row-1));
            }
            rel(space,ties_matrix(col,row) >> (O_matrix(col,row+1) >= O_matrix(col+1,row+1)));
        }
    }
}

private:

ColumnOrderingConstraint(const bool share, OperatorSpace& space, ColumnOrderingConstraint& my)
    : number_of_ties(my.number_of_ties)
{
    ties.update(space,share,my.ties);
}
//@+node:gcross.20101229110857.2452: *3* copy
virtual auto_ptr<OperatorSpace::AuxiliaryData> copy(const bool share, OperatorSpace& space) {
    return wrapAutoPtr(dynamic_cast<OperatorSpace::AuxiliaryData*>(new ColumnOrderingConstraint(share,space,*this)));
}
//@-others
};
//@+node:gcross.20101229110857.1679: ** Functions
//@+node:gcross.20101229110857.1680: *3* postColumnOrderingConstraint
void postColumnOrderingConstraint(OperatorSpace& space, const unsigned int start, const unsigned int end) {
    if(end-start < 2) return;
    space.attachAuxiliaryData(wrapAutoPtr(static_cast<OperatorSpace::AuxiliaryData*>(new ColumnOrderingConstraint(space,start,end))));
}
//@-others

}
//@-leo

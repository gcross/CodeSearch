//@+leo-ver=5-thin
//@+node:gcross.20101229110857.2512: * @thin ordering.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101229110857.2513: ** << Includes >>
#include <boost/foreach.hpp>

#include "constraints/ordering.hpp"
#include "operator_space.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101229110857.2514: ** << Usings >>
using namespace Gecode;
//@-<< Usings >>

//@+others
//@+node:gcross.20101229110857.2515: ** class OrderingConstraintData
class OrderingConstraintData : public OperatorSpace::AuxiliaryData {
//@+others
//@+node:gcross.20101229110857.2524: *3* (friends)
friend BoolVarArgs postOrderingConstraint(OperatorSpace& space, IntMatrix variables, BoolVarArgs initial_ties);
//@+node:gcross.20101229110857.2516: *3* (fields)
private:
    const unsigned int number_of_rows, number_of_cols;
    BoolVarArray ties;
//@+node:gcross.20101229110857.2517: *3* (constructors)
public:

OrderingConstraintData(OperatorSpace& space, IntMatrix variables, BoolVarArgs initial_ties)
    : number_of_rows(variables.height())
    , number_of_cols(variables.width())
    , ties(space,(number_of_rows+1)*(number_of_cols-1),0,1)
{
    assert(variables.height() > 0);
    assert(variables.width() > 1);

    BoolMatrix ties_matrix = getTiesMatrix();

    if(initial_ties.size() > 0) {
        assert(initial_ties.size() == variables.width()-1);
        BOOST_FOREACH(const unsigned int i, irange(0u,number_of_cols-1)) {
            rel(space,ties_matrix(i,0),IRT_EQ,initial_ties[i]);
        }
    } else {
        BOOST_FOREACH(const unsigned int i, irange(0u,number_of_cols-1)) {
            rel(space,ties_matrix(i,0),IRT_EQ,1);
        }
    }

    BOOST_FOREACH(const unsigned int row, irange(0u,number_of_rows)) {
        BOOST_FOREACH(const unsigned int col, irange(0u,number_of_cols-1)) {
            ties_matrix(col,row+1) = expr(space,ties_matrix(col,row) && variables(col,row) == variables(col+1,row));
            rel(space,ties_matrix(col,row) >> (variables(col,row) >= variables(col+1,row)));
        }
    }
}

private:

OrderingConstraintData(const bool share, OperatorSpace& space, OrderingConstraintData& my)
    : number_of_rows(my.number_of_rows)
    , number_of_cols(my.number_of_cols)
{
    ties.update(space,share,my.ties);
}
//@+node:gcross.20101229110857.2518: *3* copy
virtual auto_ptr<OperatorSpace::AuxiliaryData> copy(const bool share, OperatorSpace& space) {
    return wrapAutoPtr(dynamic_cast<OperatorSpace::AuxiliaryData*>(new OrderingConstraintData(share,space,*this)));
}
//@+node:gcross.20101229110857.2523: *3* getFinalTies
BoolVarArgs getFinalTies() {
    return getTiesMatrix().row(number_of_rows);
}
//@+node:gcross.20101229110857.2521: *3* getTiesMatrix
BoolMatrix getTiesMatrix() {
    return BoolMatrix(ties,number_of_cols-1,number_of_rows+1);
}
//@-others
};
//@+node:gcross.20101231214817.2187: ** class BoolOrderingData
class BoolOrderingData : public OperatorSpace::AuxiliaryData {
//@+others
//@+node:gcross.20101231214817.2188: *3* (friends)
friend BoolVarArgs postOrderingConstraint(OperatorSpace& space, BoolMatrix variables, BoolVarArgs initial_ties);
//@+node:gcross.20101231214817.2189: *3* (fields)
private:
    IntVarArray variables;
//@+node:gcross.20101231214817.2190: *3* (constructors)
public:

BoolOrderingData(OperatorSpace& space, BoolVarArgs bool_variables)
    : variables(space,bool_variables.size(),0,1)
{
    BOOST_FOREACH(const unsigned int i, irange(0u,(unsigned int)bool_variables.size())) {
        channel(space,bool_variables[i],variables[i]);
    }
}

private:

BoolOrderingData(const bool share, OperatorSpace& space, BoolOrderingData& my)
{
    variables.update(space,share,my.variables);
}
//@+node:gcross.20101231214817.2191: *3* copy
virtual auto_ptr<OperatorSpace::AuxiliaryData> copy(const bool share, OperatorSpace& space) {
    return wrapAutoPtr(dynamic_cast<OperatorSpace::AuxiliaryData*>(new BoolOrderingData(share,space,*this)));
}
//@-others
};
//@+node:gcross.20101231214817.2195: ** function postOrderingConstraint
BoolVarArgs postOrderingConstraint(OperatorSpace& space, IntMatrix variables, BoolVarArgs initial_ties) {
    if(variables.width() <= 1 || variables.height() <= 0) return initial_ties;
    auto_ptr<OrderingConstraintData> data(new OrderingConstraintData(space,variables,initial_ties));
    BoolVarArgs final_ties = data->getFinalTies();
    space.attachAuxiliaryData((auto_ptr<OperatorSpace::AuxiliaryData>)data);
    return final_ties;
}

BoolVarArgs postOrderingConstraint(OperatorSpace& space, BoolMatrix variables, BoolVarArgs initial_ties) {
    if(variables.width() <= 1 || variables.height() <= 0) return initial_ties;
    auto_ptr<BoolOrderingData> data(new BoolOrderingData(space,variables.get_array()));
    BoolVarArgs final_ties = postOrderingConstraint(space,IntMatrix(data->variables,variables.width(),variables.height()),initial_ties);
    space.attachAuxiliaryData((auto_ptr<OperatorSpace::AuxiliaryData>)data);
    return final_ties;
}
//@-others

}
//@-leo

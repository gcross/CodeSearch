//@+leo-ver=5-thin
//@+node:gcross.20101231214817.2240: * @thin constraints.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20101231214817.2241: ** << Includes >>
#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <map>

#include "boost/local/function.hpp"
#include "constraints.hpp"
#include "constraints/standard_form.hpp"
#include "operator_space.hpp"
#include "solution_iterator.hpp"
#include "utilities.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101231214817.2242: ** << Usings >>
using namespace Gecode;
using namespace boost;
using namespace boost::assign;
//@-<< Usings >>

//@+<< Function declaration >>
//@+node:gcross.20110112003748.1557: ** << Function declaration >>
StandardFormTies postColumnOrderingConstraints(
      OperatorSpace& space
    , const StandardFormParameters& parameters
);

void postEveryColumnHasXConstraint(
      OperatorSpace& space
    , const StandardFormParameters& parameters
);

void postEveryColumnHasZConstraint(
      OperatorSpace& space
    , const StandardFormParameters& parameters
);

StandardFormTies postRowOrderingConstraints(
      function<BoolVarArgs
        ( OperatorSpace& space
        , unsigned int upper_bound
        , IntMatrix region
        , BoolVarArgs initial_ties
        )
      > postConstraintForRegion
    , OperatorSpace& space
    , const StandardFormParameters& parameters
    , StandardFormTies initial_ties=no_standard_form_ties
);

void postYBeforeXConstraints(
      OperatorSpace& space
    , const StandardFormParameters& parameters
);
//@-<< Function declaration >>

//@+others
//@+node:gcross.20110112003748.1550: ** Values
typedef map<
     Constraint
    ,function<void
        ( OperatorSpace& space
        , const StandardFormParameters& parameters
        )
      >
    > ConstraintMap ;

const static ConstraintMap constraint_posters =
        map_list_of
            (EveryColumnHasX,postEveryColumnHasXConstraint)
            (EveryColumnHasZ,postEveryColumnHasZConstraint)
            (StandardForm,postStandardFormConstraint)
            (YBeforeX,postYBeforeXConstraints)
        ;

typedef map<
     Constraint
    ,function<BoolVarArgs
        ( OperatorSpace& space
        , unsigned int upper_bound
        , IntMatrix region
        , BoolVarArgs initial_ties
        )
      >
    > RowOrderingConstraintMap;

const static RowOrderingConstraintMap row_ordering_constraint_posters =
        map_list_of
            (WeightRowOrdering,postWeightRowOrderingConstraintOnRegion)
        ;

extern const set<Constraint> all_constraints =
    list_of (ColumnOrdering)
            (EveryColumnHasX)
            (EveryColumnHasZ)
            (StandardForm)
            (WeightRowOrdering)
            (YBeforeX)
    ;
//@+node:gcross.20110112003748.1547: ** Functions
//@+node:gcross.20101231214817.2245: *3* createConstrainedSpace
auto_ptr<OperatorSpace> createConstrainedSpace(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , const set<Constraint>& constraints
    , const StandardFormParameters& parameters
) {
    auto_ptr<OperatorSpace> space(new OperatorSpace(number_of_qubits,number_of_operators));
    StandardFormTies current_ties = no_standard_form_ties;
    BOOST_FOREACH(const Constraint constraint, constraints) {
        // Special case: column ordering
        if(constraint == ColumnOrdering) {
            assert(current_ties.first.size() == 0);
            assert(current_ties.second.size() == 0);
            current_ties = postColumnOrderingConstraints(*space,parameters);
            continue;
        }
        {// Special case: row ordering
            RowOrderingConstraintMap::const_iterator poster = row_ordering_constraint_posters.find(constraint);
            if(poster != row_ordering_constraint_posters.end()) {
                current_ties = postRowOrderingConstraints(
                     poster->second
                    ,*space
                    ,parameters
                    ,current_ties
                );
                continue;
            }
        }
        {// Normal case
            ConstraintMap::const_iterator poster = constraint_posters.find(constraint);
            assert(poster != constraint_posters.end());
            poster->second(*space,parameters);
            continue;
        }
        assert(!"Should never make it here");
    }
    return space;
}
//@+node:gcross.20110119165120.1744: *3* forEachStandardForm
void forEachStandardForm(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , const set<Constraint>& constraints
    , function<void (const StandardFormParameters& parameters
                    ,auto_ptr<OperatorSpace> space
                    )
              > checkAllSolutions
    ) {
        BOOST_FOREACH(const StandardFormParameters& parameters, generateStandardFormsFor(number_of_qubits,number_of_operators)) {
            checkAllSolutions(
                 parameters
                ,createConstrainedSpace(
                     number_of_qubits
                    ,number_of_operators
                    ,constraints
                    ,parameters
                 )
            );
        }
}
//@+node:gcross.20110119165120.1740: *3* forEachStandardFormSolution
void forEachStandardFormSolution(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , const set<Constraint>& constraints
    , function<void (const StandardFormParameters& parameters
                    ,const OperatorSpace& space
                    )
              > checkSolution
) {
    void BOOST_LOCAL_FUNCTION_PARAMS(
        (const StandardFormParameters& parameters)
        (auto_ptr<OperatorSpace> initial_space)
        (const bind checkSolution)
    ) {
        for_each(
             generateSolutionsFor(initial_space)
            ,bind(checkSolution,parameters,_1)
        );
    } BOOST_LOCAL_FUNCTION_NAME(checkAllSolutions)
    forEachStandardForm(
         number_of_qubits
        ,number_of_operators
        ,constraints
        ,checkAllSolutions
    );
}
//@+node:gcross.20110121100120.1573: *3* gatherAllCodesForConstraints
set<Code> gatherAllCodesForConstraints(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , const set<Constraint> constraints
    , const bool ignore_solutions_with_trivial_columns
) {
    set<Code> all_codes;

    void BOOST_LOCAL_FUNCTION_PARAMS(
        (const StandardFormParameters& parameters)
        (auto_ptr<OperatorSpace> initial_space)
        (const bind ignore_solutions_with_trivial_columns)
        (bind &all_codes)
    ) {
        const set<Code> codes = gatherCodes(initial_space,ignore_solutions_with_trivial_columns);
        all_codes.insert(codes.begin(),codes.end());
    } BOOST_LOCAL_FUNCTION_NAME(process)

    forEachStandardForm(
         number_of_qubits
        ,number_of_operators
        ,constraints
        ,process
    );

    return all_codes;
}
//@+node:gcross.20110112003748.1559: *3* postColumnOrderingConstraintOnRegion
BoolVarArgs postColumnOrderingConstraintOnRegion(OperatorSpace& space, BoolMatrix variables, BoolVarArgs initial_ties) {
    return postOrderingConstraint(space,variables,initial_ties);
}
//@+node:gcross.20110112003748.1552: *3* postColumnOrderingConstraints
StandardFormTies postColumnOrderingConstraints(
      OperatorSpace& space
    , const StandardFormParameters& parameters
) {
    const unsigned int
          number_of_qubits = space.number_of_qubits
        , number_of_operators = space.number_of_operators
        , x_bit_diagonal_size = parameters.x_bit_diagonal_size
        , z_bit_diagonal_size = parameters.z_bit_diagonal_size
        ;
    BoolMatrix X_matrix = space.getXMatrix()
             , Z_matrix = space.getZMatrix()
             ;
    postOrderingConstraint(
         space
        ,X_matrix.slice(
             x_bit_diagonal_size
            ,number_of_qubits
            ,0
            ,x_bit_diagonal_size
         )
        ,postOrderingConstraint(
             space
            ,Z_matrix.slice(
                 x_bit_diagonal_size
                ,number_of_qubits
                ,0
                ,number_of_operators
             )
         )
    );
    return make_pair(
         BoolVarArgs()
        ,postOrderingConstraint(
             space
            ,Z_matrix.slice(
                 z_bit_diagonal_size
                ,x_bit_diagonal_size
                ,x_bit_diagonal_size
                ,number_of_operators
             )
            ,postOrderingConstraint(
                 space
                ,Z_matrix.slice(
                     z_bit_diagonal_size
                    ,x_bit_diagonal_size
                    ,0
                    ,z_bit_diagonal_size
                 )
             )
         )
    );
}
//@+node:gcross.20110121100120.1550: *3* postEveryColumnHasOConstraintOnRegion
void postEveryColumnHasOConstraintOnRegion(
      int O
    , OperatorSpace& space
    , IntMatrix region
) {
    BOOST_FOREACH(const unsigned int col, irange(0u,(unsigned int)region.width())) {
        BoolVarArgs equals_O(space,region.height(),0,1);
        BOOST_FOREACH(const unsigned int row, irange(0u,(unsigned int)region.height())) {
            rel(space,region(col,row),IRT_EQ,O,equals_O[row]);
        }
        rel(space,BOT_OR,equals_O,1);
    }
}
//@+node:gcross.20110121100120.1552: *3* postEveryColumnHasXConstraint
void postEveryColumnHasXConstraint(
      OperatorSpace& space
    , const StandardFormParameters& parameters
) {
    postEveryColumnHasOConstraintOnRegion(X,space,space.getOMatrix().slice(
         parameters.x_bit_diagonal_size
        ,space.number_of_qubits
        ,0u
        ,parameters.x_bit_diagonal_size
    ));
}
//@+node:gcross.20110121100120.1554: *3* postEveryColumnHasZConstraint
void postEveryColumnHasZConstraint(
      OperatorSpace& space
    , const StandardFormParameters& parameters
) {
    postEveryColumnHasOConstraintOnRegion(Z,space,space.getOMatrix().slice(
         parameters.z_bit_diagonal_size
        ,space.number_of_qubits
        ,0u
        ,space.number_of_operators
    ));
}
//@+node:gcross.20110112003748.1556: *3* postOrderingConstraint
BoolVarArgs postOrderingConstraint(OperatorSpace& space, IntMatrix variables, BoolVarArgs initial_ties) {
    if(variables.width() <= 1 || variables.height() <= 0) return initial_ties;

    const unsigned int number_of_cols = variables.width()
                     , number_of_rows = variables.height()
                     ;

    BoolVarArgs ties(space,(number_of_rows+1)*(number_of_cols-1),0,1);
    BoolMatrix ties_matrix(ties,number_of_cols-1,number_of_rows+1);

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

    return ties_matrix.row(number_of_rows);
}

BoolVarArgs postOrderingConstraint(OperatorSpace& space, BoolMatrix variables, BoolVarArgs initial_ties) {
    return postOrderingConstraint(space,channelMatrix(space,variables),initial_ties);
}
//@+node:gcross.20110112003748.1549: *3* postRowOrderingConstraints
StandardFormTies postRowOrderingConstraints(
      function<BoolVarArgs
        ( OperatorSpace& space
        , unsigned int upper_bound
        , IntMatrix region
        , BoolVarArgs initial_ties
        )
      > postConstraintForRegion
    , OperatorSpace& space
    , const StandardFormParameters& parameters
    , StandardFormTies initial_ties
) {
    const unsigned int
          number_of_qubits = space.number_of_qubits
        , number_of_operators = space.number_of_operators
        , x_bit_diagonal_size = parameters.x_bit_diagonal_size
        , z_bit_diagonal_size = parameters.z_bit_diagonal_size
        ;
    BoolMatrix Z_matrix = space.getZMatrix();
    IntMatrix  O_matrix = space.getOMatrix();
    return make_pair(
          postConstraintForRegion(
              space
            , 2
            , channelMatrix(space,Z_matrix.slice(
                  z_bit_diagonal_size
                , number_of_qubits
                , x_bit_diagonal_size
                , number_of_operators
              ))
            , postConstraintForRegion(
                  space
                , 2
                , channelMatrix(space,Z_matrix.slice(
                      z_bit_diagonal_size
                    , x_bit_diagonal_size
                    , 0
                    , z_bit_diagonal_size
                  ))
                , postConstraintForRegion(
                      space
                    , 4
                    , O_matrix.slice(
                          x_bit_diagonal_size
                        , number_of_qubits
                        , 0
                        , z_bit_diagonal_size
                      )
                    , initial_ties.first
                  )
              )
          )
        , postConstraintForRegion(
              space
            , 2
            , channelMatrix(space,Z_matrix.slice(
                  x_bit_diagonal_size
                , number_of_qubits
                , z_bit_diagonal_size
                , x_bit_diagonal_size
              ))
            , initial_ties.second
          )
    );
}
//@+node:gcross.20110112003748.1554: *3* postWeightRowOrderingConstraintOnRegion
BoolVarArgs postWeightRowOrderingConstraintOnRegion(
      OperatorSpace& space
    , const unsigned int maximum_value
    , const IntMatrix region
    , const BoolVarArgs initial_ties
) {
    IntVarArgs region_vars = region.get_array();
    BoolVarArgs non_trivial(space,region_vars.size(),0,1);
    BOOST_FOREACH(const unsigned int i, irange(0u,(unsigned int)region_vars.size())) {
        rel(space,region_vars[i],IRT_GR,0,non_trivial[i]);
    }

    IntVarArgs weights(space,region.height(),0,region.width());
    BoolMatrix non_trivial_matrix(non_trivial,region.width(),region.height());
    BOOST_FOREACH(const unsigned int i, irange(0u,(unsigned int)region.height())) {
        linear(space,non_trivial_matrix.row(i),IRT_EQ,weights[i]);
    }

    return postOrderingConstraint(
         space
        ,IntMatrix(
             weights
            ,weights.size()
            ,1
         )
        ,initial_ties
    );
}
//@+node:gcross.20110120115216.2054: *3* postXAndZConstraintOnRegion
void postXAndZConstraintOnRegion(
      OperatorSpace& space
    , IntMatrix region
) {
    static DFA::Transition t[] =
        {{0,0,0}
        ,{0,1,1}
        ,{0,2,2}
        ,{0,3,0}
        ,{1,0,1}
        ,{1,1,1}
        ,{1,2,3}
        ,{1,3,1}
        ,{2,0,2}
        ,{2,1,3}
        ,{2,2,2}
        ,{2,3,2}
        ,{3,0,3}
        ,{3,1,3}
        ,{3,2,3}
        ,{3,3,3}
        ,{-1,-1,-1}
        };
    static int f[] = {3,-1};
    static DFA d(0,t,f);

    BOOST_FOREACH(const unsigned int col, irange(0u,(unsigned int)region.width())) {
        extensional(space,region.col(col),d);
    }
}
//@+node:gcross.20110120115216.2056: *3* postXAndZConstraints
void postXAndZConstraints(
      OperatorSpace& space
    , const StandardFormParameters& parameters
) {
    postXAndZConstraintOnRegion(
         space
        ,space.getOMatrix().slice(
             parameters.z_bit_diagonal_size
            ,space.number_of_qubits
            ,0u
            ,space.number_of_operators
         )
    );
}
//@+node:gcross.20110120115216.2113: *3* postYBeforeXConstraintOnRegion
void postYBeforeXConstraintOnRegion(
      OperatorSpace& space
    , IntMatrix region
) {
    static DFA::Transition t[] =
        {{0,0,0}
        ,{0,1,1}
        ,{0,2,0}
        ,{0,3,2}
        ,{1,0,1}
        ,{1,1,1}
        ,{1,2,1}
        ,{1,3,1}
        ,{2,0,2}
        ,{2,1,2}
        ,{2,2,2}
        ,{2,3,2}
        ,{-1,-1,-1}
        };
    static int f[] = {0,1,-1};
    static DFA d(0,t,f);

    BOOST_FOREACH(const unsigned int col, irange(0u,(unsigned int)region.width())) {
        extensional(space,region.col(col),d);
    }
}
//@+node:gcross.20110120115216.2117: *3* postYBeforeXConstraints
void postYBeforeXConstraints(
      OperatorSpace& space
    , const StandardFormParameters& parameters
) {
    postYBeforeXConstraintOnRegion(
         space
        ,space.getOMatrix().slice(
             parameters.x_bit_diagonal_size
            ,space.number_of_qubits
            ,0u
            ,parameters.x_bit_diagonal_size
         )
    );
}
//@-others

}
//@-leo

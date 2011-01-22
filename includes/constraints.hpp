//@+leo-ver=5-thin
//@+node:gcross.20101231214817.2217: * @thin constraints.hpp
//@@language cplusplus

#ifndef CONSTRAINTS_HPP
#define CONSTRAINTS_HPP

//@+<< Includes >>
//@+node:gcross.20101231214817.2218: ** << Includes >>
#include <boost/function.hpp>
#include <memory>
#include <set>

#include "constraints/standard_form.hpp"
#include "operator_space.hpp"
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101231214817.2219: ** << Usings >>
using namespace CodeQuest;
using namespace Gecode;
using namespace boost;
using namespace std;
//@-<< Usings >>

//@+others
//@+node:gcross.20101231214817.2220: ** enum Constraint
enum Constraint {
      ColumnOrdering
    , NonTrivialColumns
    , StandardForm
    , WeightRowOrdering
    };

extern const set<Constraint> all_constraints;
//@+node:gcross.20101231214817.2221: ** Functions
auto_ptr<OperatorSpace> createConstrainedSpace(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , const set<Constraint>& constraints
    , const StandardFormParameters& parameters
);

void forEachStandardForm(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , const set<Constraint>& constraints
    , function<void (const StandardFormParameters& parameters
                    ,auto_ptr<OperatorSpace> space
                    )
              > f

);

void forEachStandardFormSolution(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , const set<Constraint>& constraints
    , function<void (const StandardFormParameters& parameters
                    ,const OperatorSpace& space
                    )
              > f

);

struct Code;
set<Code> gatherAllCodesForConstraints(
      const unsigned int number_of_qubits
    , const unsigned int number_of_operators
    , const set<Constraint> constraints
    , const bool ignore_solutions_with_trivial_columns=false
);

BoolVarArgs postColumnOrderingConstraintOnRegion(
      OperatorSpace& space
    , BoolMatrix variables
    , BoolVarArgs initial_ties=BoolVarArgs()
);

void postNonTrivialColumnsConstraintOnRegion(
      OperatorSpace& space
    , BoolMatrix variables
);

BoolVarArgs postOrderingConstraint(
      OperatorSpace& space
    , IntMatrix variables
    , BoolVarArgs initial_ties=BoolVarArgs()
);

BoolVarArgs postOrderingConstraint(
      OperatorSpace& space
    , BoolMatrix variables
    , BoolVarArgs initial_ties=BoolVarArgs()
);

BoolVarArgs postWeightRowOrderingConstraintOnRegion(
      OperatorSpace& space
    , const unsigned int maximum_value
    , IntMatrix region
    , BoolVarArgs initial_ties=BoolVarArgs()
);
//@-others

}

#endif
//@-leo

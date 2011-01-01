//@+leo-ver=5-thin
//@+node:gcross.20101224191604.1847: * @thin operator_space.hpp
//@@language cplusplus

#ifndef OPERATOR_SPACE_HPP
#define OPERATOR_SPACE_HPP

//@+<< Includes >>
//@+node:gcross.20101224191604.1848: ** << Includes >>
#include <boost/foreach.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/range/irange.hpp>
#include <codequest.hpp>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/search.hh>
#include <memory>
#include <vector>
//@-<< Includes >>

namespace CodeSearch {

//@+<< Usings >>
//@+node:gcross.20101224191604.1860: ** << Usings >>
using namespace CodeQuest;
using namespace Gecode;
using namespace boost;
using namespace std;
//@-<< Usings >>

//@+others
//@+node:gcross.20101224191604.1849: ** Type aliases
typedef Matrix<IntVarArgs> IntMatrix;
typedef Matrix<BoolVarArgs> BoolMatrix;
//@+node:gcross.20101224191604.1850: ** Enums
enum Pauli { I = 0, X = 1, Z = 2, Y = 3 };
//@+node:gcross.20101224191604.1851: ** struct OperatorSpace
struct OperatorSpace : public Space {

    //@+others
    //@+node:gcross.20101224191604.3443: *3* (nested classes)
    class AuxiliaryData {
        friend class OperatorSpace;
    public:
        virtual ~AuxiliaryData() {}
    private:
        virtual auto_ptr<AuxiliaryData> copy(const bool share, OperatorSpace& space) = 0;
    };
    //@+node:gcross.20101224191604.1852: *3* (fields)
    const unsigned int number_of_operators, number_of_qubits, number_of_variables;

    BoolVarArray X, Z, non_trivial;
    IntVarArray O, weights;

    ptr_vector<AuxiliaryData> attached_auxiliary_data;
    //@+node:gcross.20101224191604.1853: *3* (constructors)
    OperatorSpace(const unsigned int number_of_operators, const unsigned int number_of_qubits);
    OperatorSpace(const bool share, OperatorSpace& s);
    //@+node:gcross.20101224191604.1854: *3* (methods)
    virtual Space* copy(const bool share);

    IntMatrix getOMatrix() const { return Matrix<IntVarArgs>(O,number_of_qubits,number_of_operators); }
    BoolMatrix getXMatrix() const { return Matrix<BoolVarArgs>(X,number_of_qubits,number_of_operators); }
    BoolMatrix getZMatrix() const { return Matrix<BoolVarArgs>(Z,number_of_qubits,number_of_operators); }
    BoolMatrix getNonTrivialMatrix() const { return Matrix<BoolVarArgs>(non_trivial,number_of_qubits,number_of_operators); }

    void attachAuxiliaryData(auto_ptr<AuxiliaryData> auxiliary_data);

    //@+others
    //@+node:gcross.20101224191604.3431: *4* computeCode
    template<typename qec_t> auto_ptr<qec_t> computeCode() const throw() {
        return wrapAutoPtr(new qec_t(*getOperators<typename qec_t::operator_vector>()));
    }
    //@+node:gcross.20101224191604.3433: *4* computeOptimizedCode
    template<typename qec_t> auto_ptr<qec_t> computeOptimizedCode() const throw() {
        auto_ptr<qec_t> qec_ptr = computeCode<qec_t>();
        qec_ptr->optimize_logical_qubits();
        return qec_ptr;
    }
    //@+node:gcross.20101229110857.2549: *4* getAuxiliaryData
    template<typename T> vector<const T*> getAuxiliaryData() const {
        vector<const T*> auxiliary_data;
        BOOST_FOREACH(const AuxiliaryData& data, attached_auxiliary_data) {
            const T* casted_data = dynamic_cast<const T*>(&data);
            if(casted_data) auxiliary_data.push_back(casted_data);
        }
        return auxiliary_data;
    }
    //@+node:gcross.20101224191604.2797: *4* getOperators
    template<typename operator_vector> auto_ptr<operator_vector> getOperators() const throw() {
        auto_ptr<operator_vector> operators(new operator_vector);
        initializeOperators(*operators);
        updateOperators(*operators);
        return operators;
    }
    //@+node:gcross.20101224191604.1855: *4* initializeOperators
    template<typename operator_vector> void initializeOperators(operator_vector& operators) const {
        operators.resize(number_of_operators);
        BOOST_FOREACH(typename operator_vector::value_type& op, operators) {
            op.resize(number_of_qubits);
        }
    }
    //@+node:gcross.20101224191604.1856: *4* updateOperators
    template<typename operator_vector> void updateOperators(operator_vector& operators) const {
        unsigned int index = 0;
        BOOST_FOREACH(typename operator_vector::value_type& op, operators) {
            BOOST_FOREACH(unsigned int q, irange(0u,number_of_qubits)) {
                op.set(q,O[index].val());
                ++index;
            }
        }
    }
    //@-others
    //@-others

};
//@+node:gcross.20101224191604.4188: ** Functions
//@+node:gcross.20101224191604.4190: *3* computeCodeForOperatorSpace
template<typename qec_t> auto_ptr<qec_t> computeCodeForOperatorSpace(const OperatorSpace& space) throw() {
    return space.computeCode<qec_t>();
}
//@+node:gcross.20101224191604.4192: *3* computeOptimizedCodeForOperatorSpace
template<typename qec_t> auto_ptr<qec_t> computeOptimizedCodeForOperatorSpace(const OperatorSpace& space) throw() {
    return space.computeOptimizedCode<qec_t>();
}
//@-others

}

#endif
//@-leo

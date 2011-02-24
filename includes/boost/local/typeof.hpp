
#ifndef BOOST_LOCAL_TYPEOF_HPP_
#define BOOST_LOCAL_TYPEOF_HPP_

#include "aux_/symbol.hpp"

// Bound variable name. Expand to qualified bound type (i.e., bound variable
// type with extra const and/or & for const and/or reference binds).
// Can be used with local functions, blocks, and exits. It accepts `this`.
#define BOOST_LOCAL_TYPEOF(bound_variable) \
    BOOST_LOCAL_AUX_SYMBOL_TYPEOF_TYPE(bound_variable)

#endif // #include guard


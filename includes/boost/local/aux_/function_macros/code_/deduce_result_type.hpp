
#ifndef BOOST_LOCAL_AUX_FUNCTION_CODE_DEDUCE_RESULT_TYPE_HPP_
#define BOOST_LOCAL_AUX_FUNCTION_CODE_DEDUCE_RESULT_TYPE_HPP_

#include "../../symbol.hpp"
// Use this lib's ScopeExit impl (for TYPEOF).
#include "../../scope_exit/scope_exit.hpp"
#include <boost/type_traits.hpp>

// This must follow the result type.
#define BOOST_LOCAL_AUX_FUNCTION_CODE_DEDUCE_RESULT_TYPE( \
        id, typename_keyword) \
    /* declare function expr with preceeding result type and no param */ \
    /* result type here */ (BOOST_LOCAL_AUX_SYMBOL_DEDUCE_RESULT_TYPE(id))();\
    typedef typename_keyword ::boost::function_traits<BOOST_TYPEOF( \
            BOOST_LOCAL_AUX_SYMBOL_DEDUCE_RESULT_TYPE(id))>::result_type \
            BOOST_LOCAL_AUX_SYMBOL_RESULT_TYPE(id);

#endif // #include guard


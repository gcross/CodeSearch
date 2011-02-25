
#ifndef BOOST_LOCAL_AUX_PP_SIGN_ANY_BIND_HPP_
#define BOOST_LOCAL_AUX_PP_SIGN_ANY_BIND_HPP_

#include "params_const_bind.hpp"
#include "params_bind.hpp"
#include <boost/preprocessor/logical/bitor.hpp>
#include <boost/preprocessor/seq.hpp> // For `PP_SEQ_TAIL`.
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>
#include <boost/preprocessor/tuple/eat.hpp>

// Private //

// Return nil-seq with any bind (const or not) but not unbind.
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_ALL_BIND_SEQ_TAIL_(params, seq_macro) \
    BOOST_PP_SEQ_TAIL(seq_macro(params))

// Public //

// Expand to 1 iff `this` is bound (const or not).
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_ANY_BIND_THIS(params) \
    /* can't use PP_BITOR/PP_OR because don't expand on MSVC */ \
    BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_CONST_BIND_THIS(params), \
        1 \
    , BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_BIND_THIS(params), \
        1 \
    , \
        0 \
    ))

// Expand to 1 iff has one or more bound including `this` (const or not).
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_ANY_BIND(params) \
    /* can't use PP_BITOR/PP_OR because don't expand on MSVC */ \
    BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_CONST_BIND(params), \
        1 \
    , BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_CONST_BIND_THIS( \
            params), \
        1 \
    , BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_BIND(params), \
        1 \
    , BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_BIND_THIS(params), \
        1 \
    , \
        0 \
    ))))

// Expand to nil-seq `(NIL) ([&]var) ...` with all binds (const or not) but
// excluding `this`.
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_ALL_BIND_WITHOUT_THIS(params) \
    (BOOST_PP_NIL) \
    BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_CONST_BIND(params), \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_ALL_BIND_SEQ_TAIL_ \
    , \
        BOOST_PP_TUPLE_EAT(2) \
    )(params, BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND) \
    BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_BIND(params), \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_ALL_BIND_SEQ_TAIL_ \
    , \
        BOOST_PP_TUPLE_EAT(2) \
    )(params, BOOST_LOCAL_AUX_PP_SIGN_PARAMS_BIND)

// Expand to nil-seq `(NIL) ([&]var) ...` with all binds (const or not) and
// including `this` if bound (const or not).
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_ALL_BIND(params) \
    BOOST_LOCAL_AUX_PP_SIGN_PARAMS_ALL_BIND(params) \
    BOOST_PP_EXPR_IIF(BOOST_PP_EXPAND( /* expand for MSVC */ \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_ANY_BIND_THIS(params)), \
        (this) /* never by reference because `&this` is not valid in C++ */ \
    )

#endif // #include guard


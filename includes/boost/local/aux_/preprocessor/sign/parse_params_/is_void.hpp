
#ifndef BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_IS_VOID_HPP_
#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_IS_VOID_HPP_

#include <boost/detail/preprocessor/keyword/void.hpp>
#include <boost/preprocessor/detail/is_unary.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/seq.hpp> // For `SEQ_HEAD`.

// Private API.

#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_IS_VOID_TOKEN_(token) \
    BOOST_PP_IIF(BOOST_PP_IS_EMPTY(token), \
        1 /* handles empty params `()` as no params (C99 only) */ \
        BOOST_PP_EMPTY \
    , \
        BOOST_DETAIL_PP_KEYWORD_IS_VOID_FRONT \
    )(token)

#define BOOST_LOCLAL_AUX_PP_SIGN_PARSE_PARAMS_IS_VOID_SEQ_(params_seq) \
    BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_IS_VOID_TOKEN_( \
            BOOST_PP_SEQ_HEAD(params_seq))

// Public API.

// Check is specified parenthesized params are empty (or void) list.
// Expand to 1 iff params_seq is empty (C99 only), or `void` (supported also
// for C++ but similar to unparenthesized paramter syntax for C99), or `(void)`
// (parenthesized parameter syntax for C++).
#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_IS_VOID(parenthesized_params) \
    BOOST_PP_IIF(BOOST_PP_IS_UNARY(parenthesized_params), \
        BOOST_LOCLAL_AUX_PP_SIGN_PARSE_PARAMS_IS_VOID_SEQ_ \
    , /* else, it's a sequence */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_IS_VOID_TOKEN_ \
    )(parenthesized_params)

#endif // #include guard


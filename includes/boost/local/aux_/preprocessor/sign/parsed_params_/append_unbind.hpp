
#ifndef BOOST_LOCAL_AUX_PP_SIGN_PARSED_APPEND_UNBIND_HPP_
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_APPEND_UNBIND_HPP_

#include "../params_unbind.hpp"
#include "../params_bind.hpp"
#include "../params_const_bind.hpp"
#include <boost/preprocessor/facilities/empty.hpp>

// unbind_classified_type_and_name: [auto | register] type name
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_APPEND_UNBIND( \
        params, unbind_classified_type_and_name) \
    ( /* unbind params and defaults */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND(params) \
        /* append param (with no default -- EMPTY) */ \
        ((unbind_classified_type_and_name, BOOST_PP_EMPTY)) \
    , /* const-bind names */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND(params) \
    , /* const-bind `this` count */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND_THIS_COUNT(params) \
    , /* bind names */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_BIND(params) \
    , /* bind `this` count */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_BIND_THIS_COUNT(params) \
    , /* error message (if any) */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_ERROR(params) \
    )

#endif // #include guard


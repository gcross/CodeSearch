
#ifndef BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_APPEND_CONST_BIND_HPP_
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_APPEND_CONST_BIND_HPP_

#include "../params_unbind.hpp"
#include "../params_bind.hpp"
#include "../params_const_bind.hpp"

// const_bind_qualified_name: [&] name (with name != `this`)
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_APPEND_CONST_BIND( \
        params, const_bind_qualified_name) \
    ( /* unbind params and defaults */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND(params) \
    , /* const-bind names */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND(params) \
        (const_bind_qualified_name) /* append */ \
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


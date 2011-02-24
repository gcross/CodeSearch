
#ifndef BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_HPP_
#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_HPP_

#include "parse_params_/is_void.hpp"
#include "parse_params_/seq.hpp"
#include "parsed_params_/nil.hpp"
#include <boost/preprocessor/control/iif.hpp>

// Private API.

#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_NONE_(unused) \
    BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_NIL

// Public API.

// parenthesized_params: Empty ``, or `void`, or `(void)` for no params
//  Or, `{([auto | register] param_type param_name)[(default default_value)] 
//  | ([const] bind [&] bind_name}+` where `bind_name` can be `this` (but not
//  `&this` as usual in C++).
#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS(parenthesized_params) \
    BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_IS_VOID( \
            parenthesized_params), \
        BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_NONE_ \
    , \
        BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ \
    )(parenthesized_params)

#endif // #include guard


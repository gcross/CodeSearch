
#ifndef BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_HPP_
#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_HPP_

#include "../parsed_params_/nil.hpp"
#include "../parsed_params_/append_unbind.hpp"
#include "../parsed_params_/append_unbind_default.hpp"
#include "../parsed_params_/append_const_bind.hpp"
#include "../parsed_params_/inc_const_bind_this_count.hpp"
#include "../parsed_params_/append_bind.hpp"
#include "../parsed_params_/inc_bind_this_count.hpp"
#include "../../keyword/const_bind.hpp"
#include "../../keyword/bind.hpp"
#include <boost/detail/preprocessor/keyword/default.hpp>
#include <boost/detail/preprocessor/keyword/this.hpp>
#include <boost/preprocessor/control/iif.hpp>

// Private: Unbound.

#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_UNBIND_DEFAULT_( \
        params, elem) \
    /* elem is `default ...` where leading default is kept because default */ \
    /* value might not be alphanumeric (so it fails later CAT for checks); */ \
    /* leading default will be remove later when getting the default value */ \
    BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_APPEND_UNBIND_DEFAULT(params, elem)

#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_UNBIND_(params, elem) \
    BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_APPEND_UNBIND(params, elem)

// Private: Parse const binds.

#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_CONST_BIND_THIS_( \
        params, elem) \
    BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INC_CONST_BIND_THIS_COUNT(params)

#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_CONST_BIND_VAR_( \
        params, elem) \
    BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_APPEND_CONST_BIND(params, \
            BOOST_LOCAL_AUX_PP_KEYWORD_CONST_BIND_REMOVE_FRONT(elem))

#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_CONST_BIND_( \
        params, elem) \
    /* check from back because non `this` bounds might have `&` in front */ \
    BOOST_PP_IIF(BOOST_DETAIL_PP_KEYWORD_IS_THIS_BACK( \
            BOOST_LOCAL_AUX_PP_KEYWORD_CONST_BIND_REMOVE_FRONT(elem)), \
        BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_CONST_BIND_THIS_ \
    , \
        BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_CONST_BIND_VAR_ \
    )(params, elem)

// Private: Parse binds.

#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_BIND_VAR_(params, elem) \
    BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_APPEND_BIND(params, \
            BOOST_LOCAL_AUX_PP_KEYWORD_BIND_REMOVE_FRONT(elem))

#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_BIND_THIS_( \
        params, elem) \
    BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INC_BIND_THIS_COUNT(params)

#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_BIND_(params, elem) \
    /* check from back because non `this` bounds might have `&` in front */ \
    BOOST_PP_IIF(BOOST_DETAIL_PP_KEYWORD_IS_THIS_BACK( \
            BOOST_LOCAL_AUX_PP_KEYWORD_BIND_REMOVE_FRONT(elem)), \
        BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_BIND_THIS_ \
    , \
        BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_BIND_VAR_ \
    )(params, elem)

// Private: Parse all elements.

#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_(s, params, elem) \
    BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_KEYWORD_IS_BIND_FRONT(elem), \
        BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_BIND_ \
    , BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_KEYWORD_IS_CONST_BIND_FRONT(elem), \
        BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_CONST_BIND_ \
    , BOOST_PP_IIF(BOOST_DETAIL_PP_KEYWORD_IS_DEFAULT_FRONT(elem), \
        BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_UNBIND_DEFAULT_ \
    , \
        BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_UNBIND_ \
    )))(params, elem)

// Public API.

// Parse param seq after following precondition has been validated by caller.
// Precondition: If seq contains a default param value `... (default ...) ...`,
// the default value element is never 1st (it always has a previous elem) and
// its previous element is a unbind param (no const-bind and no bind).
#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID(params_seq) \
    BOOST_PP_SEQ_FOLD_LEFT(BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID_, \
            BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_NIL, params_seq)

#endif // #include guard



#ifndef BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_HPP_
#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_HPP_

#include "seq_validate.hpp"
#include "seq_valid.hpp"
#include "../parsed_params_/nil.hpp"
#include "../parsed_params_/set_error.hpp"
#include "../parsed_params_/validate.hpp"
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>

// Private API.

#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_YES_(params_seq, unused) \
    BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_VALIDATE( \
            BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALID(params_seq))

#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_NO_(unused, error) \
        BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_SET_ERROR( \
                BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_NIL, error)

#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_( \
        params_seq, defaults_error) \
    BOOST_PP_IIF(BOOST_PP_IS_EMPTY(defaults_error (/* expand empty */)), \
        BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_YES_ \
    , \
        BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_NO_ \
    )(params_seq, defaults_error)

// Public API.

#define BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ(params_seq) \
    BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_(params_seq, \
            BOOST_LOCAL_AUX_PP_SIGN_PARSE_PARAMS_SEQ_VALIDATE(params_seq))

#endif // #include guard


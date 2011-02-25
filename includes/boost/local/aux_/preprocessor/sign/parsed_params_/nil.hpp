
#ifndef BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_NIL_HPP_
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_NIL_HPP_

#include <boost/preprocessor/facilities/empty.hpp>

// See "index_.hpp".
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_NIL \
    ( \
        /* unbound params: nil-seq of 2-tuples */ \
        /* `((NIL, NIL)) ((classifier_type_name, default)) ...` */ \
        ( (BOOST_PP_NIL, BOOST_PP_NIL BOOST_PP_EMPTY) ) \
    , \
        /* const-binds: nil-seq of 1-tuple `(NIL) (ref_name) ...` */ \
        (BOOST_PP_NIL) \
    , \
        0 /* number of const-bind `this` */ \
    , \
        /* binds: nil-seq of 1-tuple `(NIL) (ref_name) ...` */ \
        (BOOST_PP_NIL) \
    , \
        0 /* number of bind `this` */ \
    , \
        BOOST_PP_EMPTY /* no error */ \
    )

#endif // #include guard


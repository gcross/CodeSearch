
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// IMPORTANT: Even if this file is in the "aux_" directory, its macro names
// must NOT be prefixed by "AUX" or be within the "aux" namespace. This is
// because this file is an helper to automatically implement all `function`
// template specializations which must leave outside the "aux" namespace.

#if !BOOST_PP_IS_ITERATING
#   ifndef BOOST_LOCAL_AUX_FUNCTION_HPP_
#       define BOOST_LOCAL_AUX_FUNCTION_HPP_

#       include "abstract_function.hpp"
#       include "file.hpp"
#       include "../config.hpp"
#       include <boost/preprocessor/iteration/iterate.hpp>
#       include <boost/preprocessor/repetition/repeat.hpp>
#       include <boost/preprocessor/repetition/enum.hpp>
#       include <boost/preprocessor/punctuation/comma_if.hpp>
#       include <boost/preprocessor/arithmetic/sub.hpp>
#       include <boost/preprocessor/arithmetic/inc.hpp>
#       include <boost/preprocessor/cat.hpp>
#       include <cassert>

#define BOOST_LOCAL_AUX_arg_type(z, arg_n, unused) \
    BOOST_PP_CAT(A, arg_n)

#define BOOST_LOCAL_AUX_arg_name(z, arg_n, unused) \
    BOOST_PP_CAT(a, arg_n)

#define BOOST_LOCAL_AUX_arg(z, arg_n, unused) \
    BOOST_LOCAL_AUX_arg_type(z, arg_n, unused) \
    BOOST_LOCAL_AUX_arg_name(z, arg_n, unused)

#define BOOST_LOCAL_AUX_arg_typename(z, arg_n, unused) \
    typename BOOST_LOCAL_AUX_arg_type(z, arg_n, unused)
                
#define BOOST_LOCAL_AUX_concrete_operator_call(z, defaults_n, arity) \
    inline R operator()(BOOST_PP_ENUM_ ## z(BOOST_PP_SUB(arity, defaults_n), \
                BOOST_LOCAL_AUX_arg, ~)) { \
        assert(ptr_); \
        return (*ptr_)(BOOST_PP_ENUM_ ## z(BOOST_PP_SUB(arity, defaults_n), \
                BOOST_LOCAL_AUX_arg_name, ~)); \
    }

// Iteration within the namespace.
#       define BOOST_PP_ITERATION_PARAMS_1 \
                (3, (0, BOOST_LOCAL_CONFIG_FUNCTION_ARITY_MAX, \
                BOOST_LOCAL_AUX_FILE_FUNCTION_HPP))
#       include BOOST_PP_ITERATE() // Iterate over arity.

#   undef BOOST_LOCAL_AUX_arg_type
#   undef BOOST_LOCAL_AUX_arg_name
#   undef BOOST_LOCAL_AUX_arg
#   undef BOOST_LOCAL_AUX_rg_typename
#   undef BOOST_LOCAL_AUX_concrete_operator_call

#   endif // #include guard

#elif BOOST_PP_ITERATION_DEPTH() == 1
#   define BOOST_LOCAL_AUX_arity BOOST_PP_FRAME_ITERATION(1)

#   define BOOST_PP_ITERATION_PARAMS_2 \
            (3, (0, BOOST_LOCAL_AUX_arity, \
            BOOST_LOCAL_AUX_FILE_FUNCTION_HPP))
#   include BOOST_PP_ITERATE() // Iterate over default params count.
#   undef BOOST_LOCAL_AUX_arity

#elif BOOST_PP_ITERATION_DEPTH() == 2
#   define BOOST_LOCAL_AUX_defaults BOOST_PP_FRAME_ITERATION(2)

// Specializations of boost::local::function so not in aux namespace.
namespace boost { namespace local {

template<typename R
    BOOST_PP_COMMA_IF(BOOST_LOCAL_AUX_arity)
    BOOST_PP_ENUM(BOOST_LOCAL_AUX_arity, BOOST_LOCAL_AUX_arg_typename, ~)
>
class function<
      R (BOOST_PP_ENUM(BOOST_LOCAL_AUX_arity, BOOST_LOCAL_AUX_arg_type, ~))
    , BOOST_LOCAL_AUX_defaults
> {
    typedef aux::abstract_function<
          R (BOOST_PP_ENUM(BOOST_LOCAL_AUX_arity, BOOST_LOCAL_AUX_arg_type, ~)) 
        , BOOST_LOCAL_AUX_defaults
    > abstract_function_type;

public:
    /* implicit */ function(): ptr_() {} // Default constructor.
    
    /* implicit */ function(abstract_function_type& ref): ptr_(&ref) {}
    
    function& operator=(abstract_function_type& ref) {
        ptr_ = &ref;
        return *this;
    }

    // Result operator(Arg1, ..., ArgN-1, ArgN) // iff defaults >= 0
    // Result operator(Arg1, ..., ArgN-1)       // iff defaults >= 1
    // ...                                      // etc
    BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_LOCAL_AUX_defaults), // INC for no dflt.
            BOOST_LOCAL_AUX_concrete_operator_call, BOOST_LOCAL_AUX_arity)

private:
    abstract_function_type* ptr_;
};

}} // namespace boost::local

#   undef BOOST_LOCAL_AUX_defaults

#endif // iteration


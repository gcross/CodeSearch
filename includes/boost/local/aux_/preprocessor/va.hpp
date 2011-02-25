
#ifndef BOOST_LOCAL_AUX_PP_VA_HPP_
#define BOOST_LOCAL_AUX_PP_VA_HPP_

#include <boost/config.hpp>

#if !defined(BOOST_NO_VARIADIC_MACROS) // If no variadics then no macros.

// A local copy of Edward Diener's Variadic Macro Data proposed Boost library.
#include "va_/variadic_macro_data/vmd.hpp"

#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/comparison/greater.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>

// DATA (Internal Interface) //

#define BOOST_LOCAL_AUX_PP_VA_DATA_SIZE_(...) \
    BOOST_VMD_DATA_SIZE(__VA_ARGS__)

#define BOOST_LOCAL_AUX_PP_VA_DATA_TO_SEQ_(...) \
    BOOST_VMD_DATA_TO_PP_SEQ(__VA_ARGS__)

// VA_SAME //

#define BOOST_LOCAL_AUX_PP_VA_SAME(...) __VA_ARGS__

// VA_EAT //

#define BOOST_LOCAL_AUX_PP_VA_EAT(...) /* must expand to nothing */

// SIZE //

#define BOOST_LOCAL_AUX_PP_VA_SIZE_1OR0_TOKEN_(maybe_empty) \
    BOOST_PP_IIF(BOOST_PP_IS_EMPTY(maybe_empty (/*exapnd empty */) ), 0, 1)

#define BOOST_LOCAL_AUX_PP_VA_SIZE_1OR0_(maybe_empty) \
    BOOST_PP_IIF(BOOST_PP_IS_UNARY(maybe_empty (/*exapnd empty */) ), \
        1 BOOST_LOCAL_AUX_PP_VA_EAT \
    , \
        BOOST_LOCAL_AUX_PP_VA_SIZE_1OR0_TOKEN_ \
    )(maybe_empty)

#define BOOST_LOCAL_AUX_PP_VA_SIZE_(size, ...) \
    BOOST_PP_IIF(BOOST_PP_EQUAL(size, 1), \
        BOOST_LOCAL_AUX_PP_VA_SIZE_1OR0_ \
    , \
        size BOOST_LOCAL_AUX_PP_VA_EAT \
    )(__VA_ARGS__ BOOST_PP_EMPTY)

#define BOOST_LOCAL_AUX_PP_VA_SIZE(...) \
    BOOST_LOCAL_AUX_PP_VA_SIZE_(BOOST_LOCAL_AUX_PP_VA_DATA_SIZE_( \
            __VA_ARGS__), __VA_ARGS__)

// VA_IS //

#define BOOST_LOCAL_AUX_PP_IS_VA_NOT_UNARY_(...) \
    BOOST_PP_NOT(BOOST_PP_IS_UNARY(__VA_ARGS__))

#define BOOST_LOCAL_AUX_PP_IS_VA(...) \
    BOOST_PP_IF(BOOST_PP_GREATER( /* IIF does not expand on MSVC */ \
            BOOST_LOCAL_AUX_PP_VA_SIZE(__VA_ARGS__), 1), \
        1 BOOST_LOCAL_AUX_PP_VA_EAT \
    , \
        BOOST_LOCAL_AUX_PP_IS_VA_NOT_UNARY_ \
    )(__VA_ARGS__)

// VA_TO_SEQ //

#define BOOST_LOCAL_AUX_PP_VA_TO_SEQ_NOT_EMPTY_(...) \
    BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_IS_VA(__VA_ARGS__), \
        BOOST_LOCAL_AUX_PP_VA_DATA_TO_SEQ_ \
    , \
        BOOST_LOCAL_AUX_PP_VA_SAME \
    )(__VA_ARGS__)

// Expand to `(arg0)(arg1)...` is __VA_ARGS__ is `arg0, arg1, ...` or already
// `(arg0)(arg1)...`, if __VA_ARGS__ is empty `` expand to `empty_seq`.
#define BOOST_LOCAL_AUX_PP_VA_TO_SEQ(empty_seq, ...) \
    BOOST_PP_IF(BOOST_LOCAL_AUX_PP_VA_SIZE(__VA_ARGS__), \
        BOOST_LOCAL_AUX_PP_VA_TO_SEQ_NOT_EMPTY_ \
    , \
        empty_seq BOOST_LOCAL_AUX_PP_VA_EAT \
    )(__VA_ARGS__)

#endif // BOOST_NO_VARIADIC_MACROS

#endif // #include guard



// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

/** @file
 * @brief Local blocks allow to program code that is executed accessing some of
 * the variables in scope as constants.
 */

#ifndef BOOST_LOCAL_BLOCK_HPP_
#define BOOST_LOCAL_BLOCK_HPP_

#include "function.hpp"
#include "aux_/block.hpp"
#include <boost/config.hpp> // For variadic macros.

#if defined(BOOST_NO_VARIADIC_MACROS)

/**
 * @brief This macro starts the declaration of a local block.
 *
 * This macro must be used within a declarative context, it must be followed by
 * the local block body code <c>{ ... }</c> and then by the
 * @RefMacro{CONTRACT_DETAIL_LOCAL_BLOCK_END} macro (see the Tutorial section):
 * @code
 *  { // Some declarative context.
 *      ...
 *
 *      CONTRACT_DETAIL_LOCAL_BLOCK(
 *      parenthesized_binding
 *      ) exception_specifications_optional {
 *          ... // Block body.
 *      } CONTRACT_DETAIL_LOCAL_BLOCK_END
 *
 *      ...
 *  }
 * @endcode
 *
 * Within templates, the special macro
 * @RefMacro{CONTRACT_DETAIL_LOCAL_BLOCK_TPL} must be used instead of
 * @RefMacro{CONTRACT_DETAIL_LOCAL_BLOCK}.
 *
 * @Note A <c>return;</c> instruction from within a local block body jumps to
 *  the end of the local block body and it does not return the enclosing scope.
 *
 * @Params
 * @Param{parenthesized_binding,
 *  A Boost.Preprocessor sequence that uses the parenthesized syntax to specify
 *  the variables in scope to bind (see the Grammar section).
 * }
 * @EndParams
 *
 * As usual, exception specifications can be optionally programmed before the
 * body code block (see the Advanced section).
 *
 * @See @RefSect{Tutorial} section, @RefSect{Advanced} section,
 *  @RefSect{Grammar} section, @RefMacro{CONTRACT_DETAIL_LOCAL_BLOCK_TPL},
 *  @RefMacro{CONTRACT_DETAIL_LOCAL_BLOCK_END}.
 */
#define BOOST_LOCAL_BLOCK(binding_list) \
    BOOST_LOCAL_AUX_BLOCK(BOOST_LOCAL_FUNCTION_PARAMS, binding_list)
/**
 * @brief This macro is the same as @RefMacro{CONTRACT_DETAIL_LOCAL_BLOCK} but
 *  it must be used when declaring local blocks within templates.
 *
 * @See @RefMacro{CONTRACT_DETAIL_LOCAL_BLOCK}, @RefSect{Tutorial} section.
 */
#define BOOST_LOCAL_BLOCK_TPL(binding_list) \
    BOOST_LOCAL_AUX_BLOCK(BOOST_LOCAL_FUNCTION_PARAMS_TPL, binding_list)

#else // BOOST_NO_VARIADIC_MACROS

#include "aux_/preprocessor/va.hpp"

#define BOOST_LOCAL_BLOCK(...) \
    BOOST_LOCAL_AUX_BLOCK(BOOST_LOCAL_FUNCTION_PARAMS, \
            BOOST_LOCAL_AUX_PP_VA_TO_SEQ( \
                    (void) /* for empty seq */, __VA_ARGS__))

#define BOOST_LOCAL_BLOCK_TPL(...) \
    BOOST_LOCAL_AUX_BLOCK(BOOST_LOCAL_FUNCTION_PARAMS_TPL, \
            BOOST_LOCAL_AUX_PP_VA_TO_SEQ( \
                    (void) /* for empty seq */, __VA_ARGS__))

#endif // BOOST_NO_VARIADIC_MACROS

/**
 * @brief This macro ends the definition of a local block.
 *
 * This macro must follow the local block body code <c>{ ... }</c> as
 * shown in the @RefMacro{CONTRACT_DETAIL_LOCAL_BLOCK} documentation.
 *
 * @See @RefMacro{CONTRACT_DETAIL_LOCAL_BLOCK}, @RefSect{Tutorial} section.
 */
#define BOOST_LOCAL_BLOCK_END \
    BOOST_LOCAL_AUX_BLOCK_END(__LINE__) 

#endif // #include guard


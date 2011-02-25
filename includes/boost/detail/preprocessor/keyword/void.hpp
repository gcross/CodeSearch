
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_DETAIL_PP_KEYWORD_VOID_HPP_
#define BOOST_DETAIL_PP_KEYWORD_VOID_HPP_

#include "utility/is.hpp"
#include "utility/add.hpp"
#include "utility/remove.hpp"

// These are not local macros -- DO NOT #UNDEF.
#define BOOST_DETAIL_PP_KEYWORD_VOID_IS_void (1) /* unary */
#define void_BOOST_DETAIL_PP_KEYWORD_VOID_IS (1) /* unary */
#define BOOST_DETAIL_PP_KEYWORD_VOID_REMOVE_void /* nothing */
#define void_BOOST_DETAIL_PP_KEYWORD_VOID_REMOVE /* nothing */

// Is.

#define BOOST_DETAIL_PP_KEYWORD_IS_VOID_FRONT(tokens) \
    BOOST_DETAIL_PP_KEYWORD_UTILITY_IS_FRONT(tokens, \
            BOOST_DETAIL_PP_KEYWORD_VOID_IS_)

#define BOOST_DETAIL_PP_KEYWORD_IS_VOID_BACK(token) \
    BOOST_DETAIL_PP_KEYWORD_UTILITY_IS_BACK(token, \
            _BOOST_DETAIL_PP_KEYWORD_VOID_IS)

// Rremove.

#define BOOST_DETAIL_PP_KEYWORD_VOID_REMOVE_FRONT(tokens) \
    BOOST_DETAIL_PP_KEYWORD_UTILITY_REMOVE_FRONT(tokens, \
            BOOST_DETAIL_PP_KEYWORD_IS_VOID_FRONT, \
            BOOST_DETAIL_PP_KEYWORD_VOID_REMOVE_)

#define BOOST_DETAIL_PP_KEYWORD_VOID_REMOVE_BACK(tokens) \
    BOOST_DETAIL_PP_KEYWORD_UTILITY_REMOVE_BACK(tokens, \
            BOOST_DETAIL_PP_KEYWORD_IS_VOID_BACK, \
            _BOOST_DETAIL_PP_KEYWORD_VOID_REMOVE)

// Add.

#define BOOST_DETAIL_PP_KEYWORD_VOID_ADD_FRONT(tokens) \
    BOOST_DETAIL_PP_KEYWORD_UTILITY_ADD_FRONT(tokens, \
            BOOST_DETAIL_PP_KEYWORD_IS_VOID_FRONT, \
            void)

#define BOOST_DETAIL_PP_KEYWORD_VOID_ADD_BACK(tokens) \
    BOOST_DETAIL_PP_KEYWORD_UTILITY_ADD_BACK(tokens, \
            BOOST_DETAIL_PP_KEYWORD_IS_VOID_BACK, \
            void)

#endif // #include guard



// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_DETAIL_PP_SIGN_PARSE_DESTRUCTOR_DECL_HPP_
#define BOOST_DETAIL_PP_SIGN_PARSE_DESTRUCTOR_DECL_HPP_

#include "../validate/destructor_decl.hpp"
#include "finished.hpp"
#include "postcondition.hpp"
#include "precondition.hpp"
#include "initializers.hpp"
#include "throw.hpp"
#include "copyable.hpp"
#include "volatile.hpp"
#include "const.hpp"
#include "function_params.hpp"
#include "name.hpp"
#include "result_type.hpp"
#include "virtual.hpp"
#include "static.hpp"
#include "extern.hpp"
#include "inline.hpp"
#include "explicit.hpp"
#include "requires.hpp"
#include "template.hpp"
#include "export.hpp"
#include "access.hpp"
#include "body.hpp"
#include "start.hpp"

#define BOOST_DETAIL_PP_SIGN_PARSE_DESTRUCTOR(parenthesized_syntax_seq) \
    BOOST_DETAIL_PP_SIGN_VALIDATE_DESTRUCTOR_DECL( \
    BOOST_DETAIL_PP_SIGN_PARSE_FINISHED( \
    BOOST_DETAIL_PP_SIGN_PARSE_POSTCONDITION_NOT( \
    BOOST_DETAIL_PP_SIGN_PARSE_PRECONDITION_NOT( \
    BOOST_DETAIL_PP_SIGN_PARSE_INITIALIZERS_NOT( \
    BOOST_DETAIL_PP_SIGN_PARSE_THROW_OPT( \
    BOOST_DETAIL_PP_SIGN_PARSE_COPYABLE_NOT( \
    BOOST_DETAIL_PP_SIGN_PARSE_VOLATILE_NOT( \
    BOOST_DETAIL_PP_SIGN_PARSE_CONST_NOT( \
    /* params req and then validate enforces empty params */ \
    BOOST_DETAIL_PP_SIGN_PARSE_FUNCTION_PARAMS_REQ( \
    BOOST_DETAIL_PP_SIGN_PARSE_NAME_REQ( \
    BOOST_DETAIL_PP_SIGN_PARSE_RESULT_TYPE_NOT( \
    BOOST_DETAIL_PP_SIGN_PARSE_VIRTUAL_OPT( \
    BOOST_DETAIL_PP_SIGN_PARSE_STATIC_NOT( \
    BOOST_DETAIL_PP_SIGN_PARSE_EXTERN_NOT( \
    BOOST_DETAIL_PP_SIGN_PARSE_INLINE_OPT( \
    BOOST_DETAIL_PP_SIGN_PARSE_EXPLICIT_NOT( \
    BOOST_DETAIL_PP_SIGN_PARSE_REQUIRES_NOT( \
    BOOST_DETAIL_PP_SIGN_PARSE_TEMPLATE_NOT( \
    BOOST_DETAIL_PP_SIGN_PARSE_EXPORT_NOT( \
    BOOST_DETAIL_PP_SIGN_PARSE_ACCESS_REQ( \
    BOOST_DETAIL_PP_SIGN_PARSE_BODY_BACK_REQ( \
    BOOST_DETAIL_PP_SIGN_PARSE_START_NOEMPTY( \
        parenthesized_syntax_seq, \
        ERROR_missing_destructor_signature \
    )))))))))))))))))))))))

#endif // #include guard

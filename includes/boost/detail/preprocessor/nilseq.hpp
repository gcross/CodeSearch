
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_DETAIL_PP_NILSEQ_HPP_
#define BOOST_DETAIL_PP_NILSEQ_HPP_

#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/tuple/eat.hpp>

// Utilities to handle sequences starting with `(NIL)` (to support empty seq):
//     nilseq := (BOOST_PP_NIL) [(elem0) (elem1) ...]

// NOTE: `BOOST_PP_LIST` serves a similar purpose but it could not be used in
// order to reuse signature parsing code. For example, parsed parameters are
// represented as NILSEQ so parsers code used to parse sign sequence (wich
// operate on SEQ) can be reused. Similarly, for initializers, concepts, etc.

#define BOOST_DETAIL_PP_NILSEQ_SIZE(nilseq) \
    BOOST_PP_DEC(BOOST_PP_SEQ_SIZE(nilseq)) /* DEC for leading `(NIL)` */

#define BOOST_DETAIL_PP_NILSEQ_IS_NIL(nilseq) \
    BOOST_PP_EQUAL(BOOST_DETAIL_PP_NILSEQ_SIZE(nilseq), 0)

#define BOOST_DETAIL_PP_NILSEQ_IS_NOT_NIL(nilseq) \
    BOOST_PP_NOT(BOOST_DETAIL_PP_NILSEQ_IS_NIL(nilseq))

// Assume nilseq is not NIL and removes leading `(NIL)`.
#define BOOST_DETAIL_PP_NILSEQ_TO_SEQ(nilseq) \
    BOOST_PP_SEQ_TAIL(nilseq)

#define BOOST_DETAIL_PP_NILSEQ_ELEM(n, nilseq) \
    BOOST_PP_SEQ_ELEM(BOOST_PP_INC(n), nilseq) /* INC for leading `(NIL)` */

// ENUM //

#define BOOST_DETAIL_PP_NILSEQ_ENUM_(nilseq) \
    BOOST_PP_SEQ_ENUM(BOOST_DETAIL_PP_NILSEQ_TO_SEQ(nileseq))

#define BOOST_DETAIL_PP_NILSEQ_ENUM(nilseq) \
    BOOST_PP_IIF(BOOST_DETAIL_PP_NILSEQ_IS_NIL(nilseq), \
        BOOST_PP_TUPLE_EAT(1) \
    , \
        BOOST_DETAIL_PP_NILSEQ_ENUM_ \
    )(nilseq)

// FOR_EACH_I //

#define BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I_(op, data, nilseq) \
    /* got here iif nilseq size not nil, TAIL removes leading `(NIL)` */ \
    BOOST_PP_SEQ_FOR_EACH_I(op, data, BOOST_DETAIL_PP_NILSEQ_TO_SEQ(nilseq))

#define BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I(op, data, nilseq) \
    BOOST_PP_IIF(BOOST_DETAIL_PP_NILSEQ_IS_NIL(nilseq), \
        BOOST_PP_TUPLE_EAT(3) \
    , \
        BOOST_DETAIL_PP_NILSEQ_FOR_EACH_I_ \
    )(op, data, nilseq)

#endif // #include guard


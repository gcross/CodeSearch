
#ifndef BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INDEX_HPP_
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INDEX_HPP_

// Parameter (unbound, const-bound, and bound) tuple internal representation:
//
//  ( // Unbound params tuple. 
//        ((NIL, NIL EMPTY)) (([auto | register] ptype pname, pdflt EMPTY)) ...
//      , (NIL) ([&] var) ...       // Const bound params.
//      , const_bind_this_count     // Number of const bound `this`.
//      , (NIL) ([&] var) ...       // Bound params (not const).
//      , bind_this_count           // Number of bound `this` (not const).
//  
//  )
//
// ptype, pname, pdflt: Parameter types, names, and default values.
// var: Name of a variable in scope.

#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INDEX_UNBIND                  0
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INDEX_CONST_BIND              1
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INDEX_CONST_BIND_THIS_COUNT   2
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INDEX_BIND                    3
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INDEX_BIND_THIS_COUNT         4
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INDEX_ERROR                   5
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INDEX_MAX                     6

#endif // #include guard


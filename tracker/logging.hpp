


#pragma once

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

#include <cassert>
#include <assert.h>

///**
// * @def IE_ASSERT
// * @brief Uses assert() function if NDEBUG is not defined, InferenceEngine exception otherwise
// */
//#ifdef NDEBUG
//#define IE_ASSERT(EXPRESSION)                                                    \
//    if (!(EXPRESSION))                                                           \
//    throw InferenceEngine::details::InferenceEngineException(__FILE__, __LINE__) \
//        << "AssertionFailed: " << #EXPRESSION  // NOLINT
//#else
////#include <cassert>
//#include <assert.h>
//
// /**
//  * @private
//  */
//class NullStream {
//public:
//    template <class T>
//    NullStream& operator<<(const T&) noexcept {
//        return *this;
//    }
//
//    NullStream& operator<<(std::ostream& (*)(std::ostream&)) noexcept {
//        return *this;
//    }
//};
//
//#define IE_ASSERT(EXPRESSION) \
//    assert((EXPRESSION));     \
//    NullStream()
//#endif  // NDEBUG

//////////////////////////////////////////////////////////////////////////////////////////////////






//#include <details/ie_exception.hpp>

#define PT_CHECK(cond) assert(cond) << " "

#define PT_CHECK_BINARY(actual, expected, op) \
    assert(actual op expected) << ". " \
        << actual << " vs " << expected << ".  "

#define PT_CHECK_EQ(actual, expected) PT_CHECK_BINARY(actual, expected, ==)
#define PT_CHECK_NE(actual, expected) PT_CHECK_BINARY(actual, expected, !=)
#define PT_CHECK_LT(actual, expected) PT_CHECK_BINARY(actual, expected, <)
#define PT_CHECK_GT(actual, expected) PT_CHECK_BINARY(actual, expected, >)
#define PT_CHECK_LE(actual, expected) PT_CHECK_BINARY(actual, expected, <=)
#define PT_CHECK_GE(actual, expected) PT_CHECK_BINARY(actual, expected, >=)


/////////////////////////////////////////////////////////////////////////////

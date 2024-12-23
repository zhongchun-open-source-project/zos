// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_CHECKS_H_
#define SM_BASE_CHECKS_H_

#include <sstream>
#include <string>

#include "sm/typedefs.h"

// The macros here print a message to stderr and abort under various
// conditions. All will accept additional stream messages. For example:
// SM_DCHECK_EQ(foo, bar) << "I'm printed when foo != bar.";
//
// - SM_CHECK(x) is an assertion that x is always true, and that if it isn't,
//   it's better to terminate the process than to continue. During development,
//   the reason that it's better to terminate might simply be that the error
//   handling code isn't in place yet; in production, the reason might be that
//   the author of the code truly believes that x will always be true, but that
//   she recognizes that if she is wrong, abrupt and unpleasant process
//   termination is still better than carrying on with the assumption violated.
//
//   SM_CHECK always evaluates its argument, so it's OK for x to have side
//   effects.
//
// - SM_DCHECK(x) is the same as SM_CHECK(x)---an assertion that x is always
//   true---except that x will only be evaluated in debug builds; in production
//   builds, x is simply assumed to be true. This is useful if evaluating x is
//   expensive and the expected cost of failing to detect the violated
//   assumption is acceptable. You should not handle cases where a production
//   build fails to spot a violated condition, even those that would result in
//   crashes. If the code needs to cope with the error, make it cope, but don't
//   call SM_DCHECK; if the condition really can't occur, but you'd sleep
//   better at night knowing that the process will suicide instead of carrying
//   on in case you were wrong, use SM_CHECK instead of SM_DCHECK.
//
//   SM_DCHECK only evaluates its argument in debug builds, so if x has visible
//   side effects, you need to write e.g.
//     bool w = x; SM_DCHECK(w);
//
// - SM_CHECK_EQ, _NE, _GT, ..., and SM_DCHECK_EQ, _NE, _GT, ... are
//   specialized variants of SM_CHECK and SM_DCHECK that print prettier
//   messages if the condition doesn't hold. Prefer them to raw SM_CHECK and
//   SM_DCHECK.
//
// - FATAL() aborts unconditionally.
//
// TODO(ajm): Ideally, checks.h would be combined with logging.h, but
// consolidation with system_wrappers/logging.h should happen first.

namespace sm {

// Helper macro which avoids evaluating the arguments to a stream if
// the condition doesn't hold.
#define SM_LAZY_STREAM(stream, condition)                                    \
  !(condition) ? static_cast<void>(0) : FatalMessageVoidify() & (stream)

// The actual stream used isn't important. We reference condition in the code
// but don't evaluate it; this is to avoid "unused variable" warnings (we do so
// in a particularly convoluted way with an extra ?: because that appears to be
// the simplest construct that keeps Visual Studio from complaining about
// condition being unused).
#define SM_EAT_STREAM_PARAMETERS(condition) \
  (true ? true : !(condition))               \
      ? static_cast<void>(0)                 \
      : FatalMessageVoidify() & FatalMessage("", 0).stream()

// SM_CHECK dies with a fatal error if condition is not true.  It is *not*
// controlled by NDEBUG, so the check will be executed regardless of
// compilation mode.
//
// We make sure SM_CHECK et al. always evaluates their arguments, as
// doing SM_CHECK(FunctionWithSideEffect()) is a common idiom.
#define SM_CHECK(condition)                                      \
  SM_LAZY_STREAM(FatalMessage(__FILE__, __LINE__).stream(), \
                  !(condition))                                   \
      << "Check failed: " #condition << std::endl << "# "

// Helper macro for binary operators.
// Don't use this macro directly in your code, use SM_CHECK_EQ et al below.
//
// TODO(akalin): Rewrite this so that constructs like if (...)
// SM_CHECK_EQ(...) else { ... } work properly.
#define SM_CHECK_OP(name, op, val1, val2)                                 \
  if (std::string* _result =                                               \
          Check##name##Impl((val1), (val2), #val1 " " #op " " #val2)) \
    FatalMessage(__FILE__, __LINE__, _result).stream()

// Build the error message string.  This is separate from the "Impl"
// function template because it is not performance critical and so can
// be out of line, while the "Impl" code should be inline.  Caller
// takes ownership of the returned string.
template<class t1, class t2>
std::string* MakeCheckOpString(const t1& v1, const t2& v2, const char* names) {
  std::ostringstream ss;
  ss << names << " (" << v1 << " vs. " << v2 << ")";
  std::string* msg = new std::string(ss.str());
  return msg;
}

// MSVC doesn't like complex extern templates and DLLs.
#if !defined(COMPILER_MSVC)
// Commonly used instantiations of MakeCheckOpString<>. Explicitly instantiated
// in logging.cc.
extern template std::string* MakeCheckOpString<int, int>(
    const int&, const int&, const char* names);
extern template
std::string* MakeCheckOpString<unsigned long, unsigned long>(
    const unsigned long&, const unsigned long&, const char* names);
extern template
std::string* MakeCheckOpString<unsigned long, unsigned int>(
    const unsigned long&, const unsigned int&, const char* names);
extern template
std::string* MakeCheckOpString<unsigned int, unsigned long>(
    const unsigned int&, const unsigned long&, const char* names);
extern template
std::string* MakeCheckOpString<std::string, std::string>(
    const std::string&, const std::string&, const char* name);
#endif

// Helper functions for SM_CHECK_OP macro.
// The (int, int) specialization works around the issue that the compiler
// will not instantiate the template version of the function on values of
// unnamed enum type - see comment below.
#define DEFINE_SM_CHECK_OP_IMPL(name, op)                                   \
  template <class t1, class t2>                                              \
  inline std::string* Check##name##Impl(const t1& v1, const t2& v2,          \
                                        const char* names) {                 \
    if (v1 op v2)                                                            \
      return NULL;                                                           \
    else                                                                     \
      return MakeCheckOpString(v1, v2, names);                          \
  }                                                                          \
  inline std::string* Check##name##Impl(int v1, int v2, const char* names) { \
    if (v1 op v2)                                                            \
      return NULL;                                                           \
    else                                                                     \
      return MakeCheckOpString(v1, v2, names);                          \
  }
DEFINE_SM_CHECK_OP_IMPL(EQ, ==)
DEFINE_SM_CHECK_OP_IMPL(NE, !=)
DEFINE_SM_CHECK_OP_IMPL(LE, <=)
DEFINE_SM_CHECK_OP_IMPL(LT, < )
DEFINE_SM_CHECK_OP_IMPL(GE, >=)
DEFINE_SM_CHECK_OP_IMPL(GT, > )
#undef DEFINE_SM_CHECK_OP_IMPL

#define SM_CHECK_EQ(val1, val2) SM_CHECK_OP(EQ, ==, val1, val2)
#define SM_CHECK_NE(val1, val2) SM_CHECK_OP(NE, !=, val1, val2)
#define SM_CHECK_LE(val1, val2) SM_CHECK_OP(LE, <=, val1, val2)
#define SM_CHECK_LT(val1, val2) SM_CHECK_OP(LT, < , val1, val2)
#define SM_CHECK_GE(val1, val2) SM_CHECK_OP(GE, >=, val1, val2)
#define SM_CHECK_GT(val1, val2) SM_CHECK_OP(GT, > , val1, val2)

// The SM_DCHECK macro is equivalent to SM_CHECK except that it only generates
// code in debug builds. It does reference the condition parameter in all cases,
// though, so callers won't risk getting warnings about unused variables.
#if (!defined(NDEBUG) || defined(DCHECK_ALWAYS_ON))
#define SM_DCHECK_IS_ON 1
#define SM_DCHECK(condition) SM_CHECK(condition)
#define SM_DCHECK_EQ(v1, v2) SM_CHECK_EQ(v1, v2)
#define SM_DCHECK_NE(v1, v2) SM_CHECK_NE(v1, v2)
#define SM_DCHECK_LE(v1, v2) SM_CHECK_LE(v1, v2)
#define SM_DCHECK_LT(v1, v2) SM_CHECK_LT(v1, v2)
#define SM_DCHECK_GE(v1, v2) SM_CHECK_GE(v1, v2)
#define SM_DCHECK_GT(v1, v2) SM_CHECK_GT(v1, v2)
#else
#define SM_DCHECK_IS_ON 0
#define SM_DCHECK(condition) SM_EAT_STREAM_PARAMETERS(condition)
#define SM_DCHECK_EQ(v1, v2) SM_EAT_STREAM_PARAMETERS((v1) == (v2))
#define SM_DCHECK_NE(v1, v2) SM_EAT_STREAM_PARAMETERS((v1) != (v2))
#define SM_DCHECK_LE(v1, v2) SM_EAT_STREAM_PARAMETERS((v1) <= (v2))
#define SM_DCHECK_LT(v1, v2) SM_EAT_STREAM_PARAMETERS((v1) < (v2))
#define SM_DCHECK_GE(v1, v2) SM_EAT_STREAM_PARAMETERS((v1) >= (v2))
#define SM_DCHECK_GT(v1, v2) SM_EAT_STREAM_PARAMETERS((v1) > (v2))
#endif

// This is identical to LogMessageVoidify but in name.
class FatalMessageVoidify {
 public:
  FatalMessageVoidify() { }
  // This has to be an operator with a precedence lower than << but
  // higher than ?:
  void operator&(std::ostream&) { }
};

#define SM_UNREACHABLE_CODE_HIT false
#define SM_NOTREACHED() SM_DCHECK(SM_UNREACHABLE_CODE_HIT)

#define FATAL() FatalMessage(__FILE__, __LINE__).stream()
// TODO(ajm): Consider adding SM_NOTIMPLEMENTED macro when
// base/logging.h and system_wrappers/logging.h are consolidated such that we
// can match the Chromium behavior.

// Like a stripped-down LogMessage from logging.h, except that it aborts.
class FatalMessage {
 public:
  FatalMessage(const char* file, int line);
  // Used for SM_CHECK_EQ(), etc. Takes ownership of the given string.
  FatalMessage(const char* file, int line, std::string* result);
  NO_RETURN ~FatalMessage();

  std::ostream& stream() { return stream_; }

 private:
  void Init(const char* file, int line);

  std::ostringstream stream_;
};

// Performs the integer division a/b and returns the result. CHECKs that the
// remainder is zero.
template <typename T>
inline T CheckedDivExact(T a, T b) {
  SM_CHECK_EQ(a % b, static_cast<T>(0));
  return a / b;
}

}  // namespace sm

#endif  // SM_BASE_CHECKS_H_

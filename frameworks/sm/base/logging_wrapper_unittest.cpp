// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/logging.h"

#include "testing/gtest/include/gtest/gtest.h"
#include "sm/base/scoped_ptr.h"
#include "sm/base/condition_variable_wrapper.h"
#include "sm/base/critical_section_wrapper.h"
#include "sm/base/sleep.h"
#include "sm/base/trace.h"

namespace sm {
namespace {

class LoggingTest : public ::testing::Test, public TraceCallback {
 public:
  virtual void Print(TraceLevel level, const char* msg, int length) {
    CriticalSectionScoped cs(crit_.get());
    // We test the length here to ensure (with high likelihood) that only our
    // traces will be tested.
    if (level_ != kTraceNone && static_cast<int>(expected_log_.str().size()) ==
        length - Trace::kBoilerplateLength - 1) {
      EXPECT_EQ(level_, level);
      EXPECT_EQ(expected_log_.str(), &msg[Trace::kBoilerplateLength]);
      level_ = kTraceNone;
      cv_->Wake();
    }
  }

 protected:
  LoggingTest()
    : crit_(CriticalSectionWrapper::CreateCriticalSection()),
      cv_(ConditionVariableWrapper::CreateConditionVariable()),
      level_(kTraceNone),
      expected_log_() {
  }

  void SetUp() {
    Trace::CreateTrace();
    Trace::SetTraceCallback(this);
  }

  void TearDown() {
    Trace::SetTraceCallback(NULL);
    Trace::ReturnTrace();
    CriticalSectionScoped cs(crit_.get());
    ASSERT_EQ(kTraceNone, level_) << "Print() was not called";
  }

  scoped_ptr<CriticalSectionWrapper> crit_;
  scoped_ptr<ConditionVariableWrapper> cv_;
  TraceLevel level_ GUARDED_BY(crit_);
  std::ostringstream expected_log_ GUARDED_BY(crit_);
};

TEST_F(LoggingTest, LogStream) {
  {
    CriticalSectionScoped cs(crit_.get());
    level_ = kTraceWarning;
    std::string msg = "Important message";
    expected_log_ << "(logging_unittest.cc:" << __LINE__ + 1 << "): " << msg;
    LOG(LS_WARNING) << msg;
    cv_->SleepCS(*crit_.get(), 2000);
  }
}

}  // namespace
}  // namespace sm

// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include <utility>

#include "sm/base/checks.h"
#include "sm/base/fake_ssl_identity.h"
#include "sm/base/gunit.h"
#include "sm/base/logging.h"
#include "sm/base/sm_certificate.h"
#include "sm/base/safe_conversions.h"
#include "sm/base/scoped_ptr.h"
#include "sm/base/ssl_identity.h"
#include "sm/base/thread.h"
#include "sm/base/time_utils.h"

namespace sm {

namespace {

static const char* kTestCertCommonName = "SMCertificateTest's certificate";

}  // namespace

class SMCertificateTest : public testing::Test {
 public:
  SMCertificateTest() {}
  ~SMCertificateTest() {}

 protected:
  // Timestamp note:
  //   All timestamps in this unittest are expressed in number of seconds since
  // epoch, 1970-01-01T00:00:00Z (UTC). The SMCertificate interface uses ms,
  // but only seconds-precision is supported by SSLCertificate. To make the
  // tests clearer we convert everything to seconds since the precision matters
  // when generating certificates or comparing timestamps.
  //   As a result, ExpiresSeconds and HasExpiredSeconds are used instead of
  // SMCertificate::Expires and ::HasExpired for ms -> s conversion.

  uint64_t NowSeconds() const {
    return TimeNanos() / kNumNanosecsPerSec;
  }

  uint64_t ExpiresSeconds(const scoped_refptr<SMCertificate>& cert) const {
    uint64_t exp_ms = cert->Expires();
    uint64_t exp_s = exp_ms / kNumMillisecsPerSec;
    // Make sure this did not result in loss of precision.
    SM_CHECK_EQ(exp_s * kNumMillisecsPerSec, exp_ms);
    return exp_s;
  }

  bool HasExpiredSeconds(const scoped_refptr<SMCertificate>& cert,
                         uint64_t now_s) const {
    return cert->HasExpired(now_s * kNumMillisecsPerSec);
  }

  // An SM_CHECK ensures that |expires_s| this is in valid range of time_t as
  // is required by SSLIdentityParams. On some 32-bit systems time_t is limited
  // to < 2^31. On such systems this will fail for expiration times of year 2038
  // or later.
  scoped_refptr<SMCertificate> GenerateCertificateWithExpires(
      uint64_t expires_s) const {
    SM_CHECK(IsValueInRangeForNumericType<time_t>(expires_s));

    SSLIdentityParams params;
    params.common_name = kTestCertCommonName;
    params.not_before = 0;
    params.not_after = static_cast<time_t>(expires_s);
    // Certificate type does not matter for our purposes, using ECDSA because it
    // is fast to generate.
    params.key_params = KeyParams::ECDSA();

    scoped_ptr<SSLIdentity> identity(SSLIdentity::GenerateForTest(params));
    return SMCertificate::Create(std::move(identity));
  }
};

TEST_F(SMCertificateTest, NewCertificateNotExpired) {
  // Generate a real certificate without specifying the expiration time.
  // Certificate type doesn't matter, using ECDSA because it's fast to generate.
  scoped_ptr<SSLIdentity> identity(
      SSLIdentity::Generate(kTestCertCommonName, KeyParams::ECDSA()));
  scoped_refptr<SMCertificate> certificate =
      SMCertificate::Create(std::move(identity));

  uint64_t now = NowSeconds();
  EXPECT_FALSE(HasExpiredSeconds(certificate, now));
  // Even without specifying the expiration time we would expect it to be valid
  // for at least half an hour.
  EXPECT_FALSE(HasExpiredSeconds(certificate, now + 30*60));
}

TEST_F(SMCertificateTest, UsesExpiresAskedFor) {
  uint64_t now = NowSeconds();
  scoped_refptr<SMCertificate> certificate =
      GenerateCertificateWithExpires(now);
  EXPECT_EQ(now, ExpiresSeconds(certificate));
}

TEST_F(SMCertificateTest, ExpiresInOneSecond) {
  // Generate a certificate that expires in 1s.
  uint64_t now = NowSeconds();
  scoped_refptr<SMCertificate> certificate =
      GenerateCertificateWithExpires(now + 1);
  // Now it should not have expired.
  EXPECT_FALSE(HasExpiredSeconds(certificate, now));
  // In 2s it should have expired.
  EXPECT_TRUE(HasExpiredSeconds(certificate, now + 2));
}

}  // namespace sm

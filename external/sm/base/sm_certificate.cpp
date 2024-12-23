// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/sm_certificate.h"

#include "sm/base/checks.h"

namespace sm {

scoped_refptr<SMCertificate> SMCertificate::Create(
    scoped_ptr<SSLIdentity> identity) {
  return new RefCountedObject<SMCertificate>(identity.release());
}

SMCertificate::SMCertificate(SSLIdentity* identity)
    : identity_(identity) {
  SM_DCHECK(identity_);
}

SMCertificate::~SMCertificate() {
}

uint64_t SMCertificate::Expires() const {
  int64_t expires = ssl_certificate().CertificateExpirationTime();
  if (expires != -1)
    return static_cast<uint64_t>(expires) * kNumMillisecsPerSec;
  // If the expiration time could not be retrieved return an expired timestamp.
  return 0;  // = 1970-01-01
}

bool SMCertificate::HasExpired(uint64_t now) const {
  return Expires() <= now;
}

const SSLCertificate& SMCertificate::ssl_certificate() const {
  return identity_->certificate();
}

}  // namespace sm

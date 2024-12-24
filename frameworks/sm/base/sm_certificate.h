// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_SM_CERTIFICATE_H_
#define SM_BASE_SM_CERTIFICATE_H_

#include "sm/base/basic_types.h"
#include "sm/base/refcount.h"
#include "sm/base/scoped_ptr.h"
#include "sm/base/scoped_ref_ptr.h"
#include "sm/base/ssl_identity.h"

namespace sm {

// A thin abstraction layer between "lower level crypto stuff" like
// SSLCertificate and SM usage. Takes ownership of some lower level objects,
// reference counting protects these from premature destruction.
class SMCertificate : public RefCountInterface {
 public:
  // Takes ownership of |identity|.
  static scoped_refptr<SMCertificate> Create(scoped_ptr<SSLIdentity> identity);

  // Returns the expiration time in ms relative to epoch, 1970-01-01T00:00:00Z.
  uint64_t Expires() const;
  // Checks if the certificate has expired, where |now| is expressed in ms
  // relative to epoch, 1970-01-01T00:00:00Z.
  bool HasExpired(uint64_t now) const;
  const SSLCertificate& ssl_certificate() const;

  // TODO(hbos): If possible, remove once SMCertificate and its
  // ssl_certificate() is used in all relevant places. Should not pass around
  // raw SSLIdentity* for the sake of accessing SSLIdentity::certificate().
  // However, some places might need SSLIdentity* for its public/private key...
  SSLIdentity* identity() const { return identity_.get(); }

 protected:
  explicit SMCertificate(SSLIdentity* identity);
  ~SMCertificate() override;

 private:
  // The SSLIdentity is the owner of the SSLCertificate. To protect our
  // ssl_certificate() we take ownership of |identity_|.
  scoped_ptr<SSLIdentity> identity_;
};

}  // namespace sm

#endif  // SM_BASE_SM_CERTIFICATE_H_

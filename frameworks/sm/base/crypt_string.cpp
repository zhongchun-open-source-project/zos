// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/crypt_string.h"

namespace sm {

size_t EmptyCryptStringImpl::GetLength() const {
  return 0;
}

void EmptyCryptStringImpl::CopyTo(char* dest, bool nullterminate) const {
  if (nullterminate) {
    *dest = '\0';
  }
}

std::string EmptyCryptStringImpl::UrlEncode() const {
  return "";
}

CryptStringImpl* EmptyCryptStringImpl::Copy() const {
  return new EmptyCryptStringImpl();
}

void EmptyCryptStringImpl::CopyRawTo(std::vector<unsigned char>* dest) const {
  dest->clear();
}

CryptString::CryptString() : impl_(new EmptyCryptStringImpl()) {
}

CryptString::CryptString(const CryptString& other)
    : impl_(other.impl_->Copy()) {
}

CryptString::CryptString(const CryptStringImpl& impl) : impl_(impl.Copy()) {
}

CryptString::~CryptString() = default;

size_t InsecureCryptStringImpl::GetLength() const {
  return password_.size();
}

void InsecureCryptStringImpl::CopyTo(char* dest, bool nullterminate) const {
  memcpy(dest, password_.data(), password_.size());
  if (nullterminate)
    dest[password_.size()] = 0;
}

std::string InsecureCryptStringImpl::UrlEncode() const {
  return password_;
}

CryptStringImpl* InsecureCryptStringImpl::Copy() const {
  InsecureCryptStringImpl* copy = new InsecureCryptStringImpl;
  copy->password() = password_;
  return copy;
}

void InsecureCryptStringImpl::CopyRawTo(
    std::vector<unsigned char>* dest) const {
  dest->resize(password_.size());
  memcpy(&dest->front(), password_.data(), password_.size());
}

};  // namespace sm

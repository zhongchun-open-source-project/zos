// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_SSL_CONFIG_H_
#define SM_BASE_SSL_CONFIG_H_

// If no preference has been indicated, default to SChannel on Windows and
// OpenSSL everywhere else, if it is available.
#if !defined(SSL_USE_SCHANNEL) && !defined(SSL_USE_OPENSSL)
#if defined(SM_WIN)

#define SSL_USE_SCHANNEL 1

#else  // defined(SM_WIN)

#if defined(HAVE_OPENSSL_SSL_H)
#define SSL_USE_OPENSSL 1
#endif

#endif  // !defined(SM_WIN)
#endif

#endif  // SM_BASE_SSL_CONFIG_H_

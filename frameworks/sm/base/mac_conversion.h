// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_MAC_CONVERSION_H_
#define SM_BASE_MAC_CONVERSION_H_

#if defined(SM_MAC) || defined(SM_IOS)

#include <CoreFoundation/CoreFoundation.h>

#include <string>

// given a CFStringRef, attempt to convert it to a C++ string.
// returns true if it succeeds, false otherwise.
// We can safely assume, given our context, that the string is
// going to be in ASCII, because it will either be an IP address,
// or a domain name, which is guaranteed to be ASCII-representable.
bool p_convertHostCFStringRefToCPPString(const CFStringRef cfstr,
                                         std::string& cppstr);

// Convert the CFNumber to an integer, putting the integer in the location
// given, and returhing true, if the conversion succeeds.
// If given a NULL or a non-CFNumber, returns false.
// This is pretty aggresive about trying to convert to int.
bool p_convertCFNumberToInt(CFNumberRef cfn, int* i);

// given a CFNumberRef, determine if it represents a true value.
bool p_isCFNumberTrue(CFNumberRef cfn);

#endif  // SM_MAC || SM_IOS

#endif  // SM_BASE_MACCONVERSION_H_

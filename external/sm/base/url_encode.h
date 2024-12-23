// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_URL_ENCODE_H_
#define SM_BASE_URL_ENCODE_H_ 

#include <string>

namespace sm {

// Decode all encoded characters. Also decode + as space.
int UrlDecode(const char *source, char *dest);

// Decode all encoded characters.
int UrlDecodeWithoutEncodingSpaceAsPlus(const char *source, char *dest);

// Encode all characters except alphas, numbers, and -_.!~*'()
// Also encode space as +.
int UrlEncode(const char *source, char *dest, unsigned max);

// Encode all characters except alphas, numbers, and -_.!~*'()
int UrlEncodeWithoutEncodingSpaceAsPlus(const char *source, char *dest,
                                        unsigned max);

// Encode only unsafe chars, including \ "^&`<>[]{}
// Also encode space as %20, instead of +
int UrlEncodeOnlyUnsafeChars(const char *source, char *dest, unsigned max);

std::string UrlDecodeString(const std::string & encoded);
std::string UrlDecodeStringWithoutEncodingSpaceAsPlus(
    const std::string & encoded);
std::string UrlEncodeString(const std::string & decoded);
std::string UrlEncodeStringWithoutEncodingSpaceAsPlus(
    const std::string & decoded);
std::string UrlEncodeStringForOnlyUnsafeChars(const std::string & decoded);

}  // namespace sm

#endif // SM_BASE_URL_ENCODE_H_
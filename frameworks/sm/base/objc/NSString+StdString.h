// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#import <Foundation/Foundation.h>

#include <string>

NS_ASSUME_NONNULL_BEGIN

@interface NSString (StdString)

@property(nonatomic, readonly) std::string stdString;

+ (std::string)stdStringForString:(NSString *)nsString;
+ (NSString *)stringForStdString:(const std::string&)stdString;

@end

NS_ASSUME_NONNULL_END

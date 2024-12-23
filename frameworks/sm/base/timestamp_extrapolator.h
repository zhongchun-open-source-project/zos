// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef BASE_TIMESTAMP_EXTRAPOLATOR_H_
#define BASE_TIMESTAMP_EXTRAPOLATOR_H_

#include "sm/base/rw_lock_wrapper.h"
#include "sm/typedefs.h"

namespace sm
{

class TimestampExtrapolator
{
public:
    explicit TimestampExtrapolator(int64_t start_ms);
    ~TimestampExtrapolator();
    void Update(int64_t tMs, uint32_t ts90khz);
    int64_t ExtrapolateLocalTime(uint32_t timestamp90khz);
    void Reset(int64_t start_ms);

private:
    void CheckForWrapArounds(uint32_t ts90khz);
    bool DelayChangeDetection(double error);
    RWLockWrapper*        _rwLock;
    double                _w[2];
    double                _pP[2][2];
    int64_t         _startMs;
    int64_t         _prevMs;
    uint32_t        _firstTimestamp;
    int32_t         _wrapArounds;
    int64_t         _prevUnwrappedTimestamp;
    int64_t         _prevWrapTimestamp;
    const double          _lambda;
    bool                  _firstAfterReset;
    uint32_t        _packetCount;
    const uint32_t  _startUpFilterDelayInPackets;

    double              _detectorAccumulatorPos;
    double              _detectorAccumulatorNeg;
    const double        _alarmThreshold;
    const double        _accDrift;
    const double        _accMaxError;
    const double        _pP11;
};

}  // namespace sm

#endif // BASE_TIMESTAMP_EXTRAPOLATOR_H_

// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_RTP_TO_NTP_H_
#define SM_BASE_RTP_TO_NTP_H_

#include <list>

#include "sm/typedefs.h"

namespace sm {

struct RtcpMeasurement {
  RtcpMeasurement();
  RtcpMeasurement(uint32_t ntp_secs, uint32_t ntp_frac, uint32_t timestamp);
  uint32_t ntp_secs;
  uint32_t ntp_frac;
  uint32_t rtp_timestamp;
};

typedef std::list<RtcpMeasurement> RtcpList;

// Updates |rtcp_list| with timestamps from the latest RTCP SR.
// |new_rtcp_sr| will be set to true if these are the timestamps which have
// never be added to |rtcp_list|.
bool UpdateRtcpList(uint32_t ntp_secs,
                    uint32_t ntp_frac,
                    uint32_t rtp_timestamp,
                    RtcpList* rtcp_list,
                    bool* new_rtcp_sr);

// Converts an RTP timestamp to the NTP domain in milliseconds using two
// (RTP timestamp, NTP timestamp) pairs.
bool RtpToNtpMs(int64_t rtp_timestamp, const RtcpList& rtcp,
                int64_t* timestamp_in_ms);

// Returns 1 there has been a forward wrap around, 0 if there has been no wrap
// around and -1 if there has been a backwards wrap around (i.e. reordering).
int CheckForWrapArounds(uint32_t rtp_timestamp, uint32_t rtcp_rtp_timestamp);

}  // namespace sm

#endif  // SM_BASE_RTP_TO_NTP_H_

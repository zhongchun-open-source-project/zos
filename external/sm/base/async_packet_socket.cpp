// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/async_packet_socket.h"

namespace sm {

PacketTimeUpdateParams::PacketTimeUpdateParams()
    : rtp_sendtime_extension_id(-1),
      srtp_auth_tag_len(-1),
      srtp_packet_index(-1) {
}

PacketTimeUpdateParams::~PacketTimeUpdateParams() = default;

AsyncPacketSocket::AsyncPacketSocket() {
}

AsyncPacketSocket::~AsyncPacketSocket() {
}

};  // namespace sm

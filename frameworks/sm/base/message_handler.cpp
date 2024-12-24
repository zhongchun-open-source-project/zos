// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/message_handler.h"
#include "sm/base/message_queue.h"

namespace sm {

MessageHandler::~MessageHandler() {
  MessageQueueManager::Clear(this);
}

} // namespace sm

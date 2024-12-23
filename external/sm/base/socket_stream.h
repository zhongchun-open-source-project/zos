// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_SOCKET_STREAM_H_
#define SM_BASE_SOCKET_STREAM_H_

#include "sm/base/async_socket.h"
#include "sm/base/common.h"
#include "sm/base/stream.h"

namespace sm {

///////////////////////////////////////////////////////////////////////////////

class SocketStream : public StreamInterface, public sigslot::has_slots<> {
 public:
  explicit SocketStream(AsyncSocket* socket);
  ~SocketStream() override;

  void Attach(AsyncSocket* socket);
  AsyncSocket* Detach();

  AsyncSocket* GetSocket() { return socket_; }

  StreamState GetState() const override;

  StreamResult Read(void* buffer,
                    size_t buffer_len,
                    size_t* read,
                    int* error) override;

  StreamResult Write(const void* data,
                     size_t data_len,
                     size_t* written,
                     int* error) override;

  void Close() override;

 private:
  void OnConnectEvent(AsyncSocket* socket);
  void OnReadEvent(AsyncSocket* socket);
  void OnWriteEvent(AsyncSocket* socket);
  void OnCloseEvent(AsyncSocket* socket, int err);

  AsyncSocket* socket_;

  SM_DISALLOW_COPY_AND_ASSIGN(SocketStream);
};

///////////////////////////////////////////////////////////////////////////////

}  // namespace sm

#endif  // SM_BASE_SOCKET_STREAM_H_

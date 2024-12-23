// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_ASYNC_SOCKET_H_
#define SM_BASE_ASYNC_SOCKET_H_

#include "sm/base/common.h"
#include "sm/base/sigslot.h"
#include "sm/base/socket.h"

namespace sm {

// TODO: Remove Socket and rename AsyncSocket to Socket.

// Provides the ability to perform socket I/O asynchronously.
class AsyncSocket : public Socket {
 public:
  AsyncSocket();
  ~AsyncSocket() override;

  AsyncSocket* Accept(SocketAddress* paddr) override = 0;

  // SignalReadEvent and SignalWriteEvent use multi_threaded_local to allow
  // access concurrently from different thread.
  // For example SignalReadEvent::connect will be called in AsyncUDPSocket ctor
  // but at the same time the SocketDispatcher maybe signaling the read event.
  // ready to read
  sigslot::signal1<AsyncSocket*,
                   sigslot::multi_threaded_local> SignalReadEvent;
  // ready to write
  sigslot::signal1<AsyncSocket*,
                   sigslot::multi_threaded_local> SignalWriteEvent;
  sigslot::signal1<AsyncSocket*> SignalConnectEvent;     // connected
  sigslot::signal2<AsyncSocket*, int> SignalCloseEvent;  // closed
};

class AsyncSocketAdapter : public AsyncSocket, public sigslot::has_slots<> {
 public:
  // The adapted socket may explicitly be NULL, and later assigned using Attach.
  // However, subclasses which support detached mode must override any methods
  // that will be called during the detached period (usually GetState()), to
  // avoid dereferencing a null pointer.
  explicit AsyncSocketAdapter(AsyncSocket* socket);
  ~AsyncSocketAdapter() override;
  void Attach(AsyncSocket* socket);
  SocketAddress GetLocalAddress() const override;
  SocketAddress GetRemoteAddress() const override;
  int Bind(const SocketAddress& addr) override;
  int Connect(const SocketAddress& addr) override;
  int Send(const void* pv, size_t cb) override;
  int SendTo(const void* pv, size_t cb, const SocketAddress& addr) override;
  int Recv(void* pv, size_t cb) override;
  int RecvFrom(void* pv, size_t cb, SocketAddress* paddr) override;
  int Listen(int backlog) override;
  AsyncSocket* Accept(SocketAddress* paddr) override;
  int Close() override;
  int GetError() const override;
  void SetError(int error) override;
  ConnState GetState() const override;
  int EstimateMTU(uint16_t* mtu) override;
  int GetOption(Option opt, int* value) override;
  int SetOption(Option opt, int value) override;

 protected:
  virtual void OnConnectEvent(AsyncSocket* socket);
  virtual void OnReadEvent(AsyncSocket* socket);
  virtual void OnWriteEvent(AsyncSocket* socket);
  virtual void OnCloseEvent(AsyncSocket* socket, int err);

  AsyncSocket* socket_;
};

}  // namespace sm

#endif  // SM_BASE_ASYNC_SOCKET_H_

// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_ASYNC_TCP_SOCKET_H_
#define SM_BASE_ASYNC_TCP_SOCKET_H_

#include "sm/base/async_packet_socket.h"
#include "sm/base/scoped_ptr.h"
#include "sm/base/socket_factory.h"

namespace sm {

// Simulates UDP semantics over TCP.  Send and Recv packet sizes
// are preserved, and drops packets silently on Send, rather than
// buffer them in user space.
class AsyncTCPSocketBase : public AsyncPacketSocket {
 public:
  AsyncTCPSocketBase(AsyncSocket* socket, bool listen, size_t max_packet_size);
  ~AsyncTCPSocketBase() override;

  // Pure virtual methods to send and recv data.
  int Send(const void *pv, size_t cb,
                   const PacketOptions& options) override = 0;
  virtual void ProcessInput(char* data, size_t* len) = 0;
  // Signals incoming connection.
  virtual void HandleIncomingConnection(AsyncSocket* socket) = 0;

  SocketAddress GetLocalAddress() const override;
  SocketAddress GetRemoteAddress() const override;
  int SendTo(const void* pv,
             size_t cb,
             const SocketAddress& addr,
             const PacketOptions& options) override;
  int Close() override;

  State GetState() const override;
  int GetOption(Socket::Option opt, int* value) override;
  int SetOption(Socket::Option opt, int value) override;
  int GetError() const override;
  void SetError(int error) override;

 protected:
  // Binds and connects |socket| and creates AsyncTCPSocket for
  // it. Takes ownership of |socket|. Returns NULL if bind() or
  // connect() fail (|socket| is destroyed in that case).
  static AsyncSocket* ConnectSocket(AsyncSocket* socket,
                                    const SocketAddress& bind_address,
                                    const SocketAddress& remote_address);
  virtual int SendRaw(const void* pv, size_t cb);
  int FlushOutBuffer();
  // Add data to |outbuf_|.
  void AppendToOutBuffer(const void* pv, size_t cb);

  // Helper methods for |outpos_|.
  bool IsOutBufferEmpty() const { return outpos_ == 0; }
  void ClearOutBuffer() { outpos_ = 0; }

 private:
  // Called by the underlying socket
  void OnConnectEvent(AsyncSocket* socket);
  void OnReadEvent(AsyncSocket* socket);
  void OnWriteEvent(AsyncSocket* socket);
  void OnCloseEvent(AsyncSocket* socket, int error);

  scoped_ptr<AsyncSocket> socket_;
  bool listen_;
  char* inbuf_, * outbuf_;
  size_t insize_, inpos_, outsize_, outpos_;

  SM_DISALLOW_COPY_AND_ASSIGN(AsyncTCPSocketBase);
};

class AsyncTCPSocket : public AsyncTCPSocketBase {
 public:
  // Binds and connects |socket| and creates AsyncTCPSocket for
  // it. Takes ownership of |socket|. Returns NULL if bind() or
  // connect() fail (|socket| is destroyed in that case).
  static AsyncTCPSocket* Create(AsyncSocket* socket,
                                const SocketAddress& bind_address,
                                const SocketAddress& remote_address);
  AsyncTCPSocket(AsyncSocket* socket, bool listen);
  ~AsyncTCPSocket() override {}

  int Send(const void* pv,
           size_t cb,
           const PacketOptions& options) override;
  void ProcessInput(char* data, size_t* len) override;
  void HandleIncomingConnection(AsyncSocket* socket) override;

 private:
  SM_DISALLOW_COPY_AND_ASSIGN(AsyncTCPSocket);
};

}  // namespace sm

#endif  // SM_BASE_ASYNC_TCP_SOCKET_H_

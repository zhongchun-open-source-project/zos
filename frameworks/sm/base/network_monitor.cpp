// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/network_monitor.h"

#include "sm/base/common.h"

namespace {
const uint32_t UPDATE_NETWORKS_MESSAGE = 1;

// This is set by NetworkMonitorFactory::SetFactory and the caller of
// NetworkMonitorFactory::SetFactory must be responsible for calling
// ReleaseFactory to destroy the factory.
NetworkMonitorFactory* network_monitor_factory = nullptr;
}  // namespace

namespace sm {
NetworkMonitorInterface::NetworkMonitorInterface() {}

NetworkMonitorInterface::~NetworkMonitorInterface() {}

NetworkMonitorBase::NetworkMonitorBase() : thread_(Thread::Current()) {}
NetworkMonitorBase::~NetworkMonitorBase() {}

void NetworkMonitorBase::OnNetworksChanged() {
  LOG(LS_VERBOSE) << "Network change is received at the network monitor";
  thread_->Post(this, UPDATE_NETWORKS_MESSAGE);
}

void NetworkMonitorBase::OnMessage(Message* msg) {
  ASSERT(msg->message_id == UPDATE_NETWORKS_MESSAGE);
  SignalNetworksChanged();
}

NetworkMonitorFactory::NetworkMonitorFactory() {}
NetworkMonitorFactory::~NetworkMonitorFactory() {}

void NetworkMonitorFactory::SetFactory(NetworkMonitorFactory* factory) {
  if (network_monitor_factory != nullptr) {
    delete network_monitor_factory;
  }
  network_monitor_factory = factory;
}

void NetworkMonitorFactory::ReleaseFactory(NetworkMonitorFactory* factory) {
  if (factory == network_monitor_factory) {
    SetFactory(nullptr);
  }
}

NetworkMonitorFactory* NetworkMonitorFactory::GetFactory() {
  return network_monitor_factory;
}

}  // namespace sm

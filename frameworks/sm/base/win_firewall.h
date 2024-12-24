// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_WIN_FIREWALL_H_
#define SM_BASE_WIN_FIREWALL_H_

#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef long HRESULT;  // Can't forward declare typedef, but don't need all win
#endif // !_HRESULT_DEFINED

struct INetFwMgr;
struct INetFwPolicy;
struct INetFwProfile;

namespace sm {

//////////////////////////////////////////////////////////////////////
// WinFirewall
//////////////////////////////////////////////////////////////////////

class WinFirewall {
 public:
  WinFirewall();
  ~WinFirewall();

  bool Initialize(HRESULT* result);
  void Shutdown();

  bool Enabled() const;
  bool QueryAuthorized(const char* filename, bool* authorized) const;
  bool QueryAuthorizedW(const wchar_t* filename, bool* authorized) const;

  bool AddApplication(const char* filename, const char* friendly_name,
                      bool authorized, HRESULT* result);
  bool AddApplicationW(const wchar_t* filename, const wchar_t* friendly_name,
                       bool authorized, HRESULT* result);

 private:
  INetFwMgr* mgr_;
  INetFwPolicy* policy_;
  INetFwProfile* profile_;
};

//////////////////////////////////////////////////////////////////////

}  // namespace sm

#endif  // SM_BASE_WIN_FIREWALL_H_

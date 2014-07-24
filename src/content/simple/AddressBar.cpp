#include "StdAfx.h"
#include "resource.h"
#include "AddressBar.h"



LRESULT CAddressBar::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // CenterWindow(GetParent());
  return TRUE;
}

LRESULT CAddressBar::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  EndDialog(wID);
  return 0;
}

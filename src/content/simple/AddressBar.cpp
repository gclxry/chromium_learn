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

LRESULT CAddressBar::OnBack(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  MessageBox(L"OnBack");
  return 0;
}

LRESULT CAddressBar::OnForward(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  MessageBox(L"OnForward");
  return 0;
}

LRESULT CAddressBar::OnReload(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  MessageBox(L"OnReload");
  return 0;
}

LRESULT CAddressBar::OnStop(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  MessageBox(L"OnStop");
  return 0;
}

void CAddressBar::SetUrl(CString url)
{
  CEdit edit;
  edit.Attach(GetDlgItem(IDC_EDIT1));
  edit.SetWindowText(url);
}

CString CAddressBar::GetUrl()
{
  CEdit edit;
  CString url;
  int nLength;
  LPTSTR pszText;

  edit.Attach(GetDlgItem(IDC_EDIT1));
  nLength = edit.GetWindowTextLength();
  pszText = url.GetBuffer(nLength+1);
  edit.GetWindowText(pszText);
  url.ReleaseBuffer(nLength);
  return url;
}
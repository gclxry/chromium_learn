#include "StdAfx.h"
#include "resource.h"
#include "AddressBar.h"

CAddressBar::CAddressBar() : m_edit(this, 1)
{

}

LRESULT CAddressBar::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // CenterWindow(GetParent());
  m_edit.SubclassWindow ( GetDlgItem(IDC_EDIT1) );
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

  HWND hEdit = GetDlgItem(IDC_EDIT1);
  if (!::IsWindow(hEdit))
  {
    return CString();
  }
  edit.Attach(hEdit);
  nLength = edit.GetWindowTextLength();
  nLength++;
  pszText = url.GetBuffer(nLength);
  edit.GetWindowText(pszText, nLength);
  url.ReleaseBuffer(nLength);
  return url;
}

LRESULT CAddressBar::OnEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  int ia = 0;
  ia++;
  if (VK_RETURN == (TCHAR)wParam)
  {
    //MessageBox(GetUrl());
    //SetMsgHandled(TRUE);
    ::PostMessage(GetParent(), WM_USER_RETURN, 0, 0);
    bHandled = TRUE;
  }
  bHandled = FALSE;

  return 0;
}
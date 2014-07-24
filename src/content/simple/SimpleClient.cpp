#include "StdAfx.h"
#include "resource.h"
#include "SimpleClient.h"


CSimpleClient::CSimpleClient()
{
  m_hBkBrush = CreateSolidBrush(RGB(255, 255, 255));
}

CSimpleClient::~CSimpleClient()
{
  if (NULL != m_hBkBrush)
  {
    DeleteObject(m_hBkBrush);
  }
}

LRESULT CSimpleClient::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // CenterWindow(GetParent());
  return TRUE;
}

LRESULT CSimpleClient::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  EndDialog(wID);
  return 0;
}


HBRUSH CSimpleClient::OnCtlColorDlg(CDCHandle dc, CWindow wnd)
{
  return (HBRUSH) m_hBkBrush;
}
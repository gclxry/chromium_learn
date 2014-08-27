#include "StdAfx.h"
#include "resource.h"
#include "SimpleTab.h"


LRESULT CSimpleTab::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  CenterWindow(GetParent());
  m_index = 1000;
  Layout();
  return TRUE;
}

LRESULT CSimpleTab::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  EndDialog(wID);
  return 0;
}

LRESULT CSimpleTab::OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

  // MessageBox(L"OnAdd");
  CButton* btn = new CButton;
  m_buttons.push_back(btn);
  RECT rc = {0,0, 0, 0};
  btn->Create(m_hWnd, rc, L"ÐÂ½¨±êÇ©Ò³", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON |  BS_NOTIFY, 0, ++m_index);

  Layout();
  return TRUE;
}

void CSimpleTab::Layout()
{
  int index = 0;
  int nWidth = 200;
  int nHeight= 25;
  for (vector<CButton*>::iterator iter = m_buttons.begin(); iter != m_buttons.end(); ++iter)
  {
    CButton* btn = *iter;
    ::MoveWindow(btn->m_hWnd, index* nWidth, 2, nWidth, nHeight, TRUE);
    index++;
  }

  ::MoveWindow(GetDlgItem(IDC_ADD), index* nWidth, 2, nHeight, nHeight, TRUE);
}

LRESULT CSimpleTab::OnButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  if (BN_CLICKED == wNotifyCode)
  {
  }

  if (BN_DBLCLK == wNotifyCode)
  {
  }
  CString temp;
  temp.Format(L"%u %u\n", wNotifyCode, wID);
  OutputDebugString(temp);

  return TRUE;
}  
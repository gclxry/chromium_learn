#pragma once
#include <vector>

using namespace std;

class CSimpleTab : public CDialogImpl<CSimpleTab>
{
public:
  enum { IDD = IDD_SIMPLE_TAB };

  BEGIN_MSG_MAP(CSimpleTab)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
    COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
    COMMAND_ID_HANDLER(IDC_ADD, OnAdd)
    COMMAND_RANGE_HANDLER(1000, 2000, OnButton)
  END_MSG_MAP()

  // Handler prototypes (uncomment arguments if needed):
  //	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  //	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  //	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  HBRUSH OnCtlColorBtn(CDCHandle dc, CButton button);

  LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
  LRESULT OnButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

  void Layout();

  int m_index;
  vector<CButton*> m_buttons;
  HBRUSH m_hButtonBk;
  HBRUSH m_hCurrentBrush;
  HWND m_current_tab;
};
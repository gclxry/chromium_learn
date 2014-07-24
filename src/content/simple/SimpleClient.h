#pragma once

class CSimpleClient : public CDialogImpl<CSimpleClient>
{
public:
  enum { IDD = IDD_CLIENT };

  BEGIN_MSG_MAP(CSimpleClient)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MSG_WM_CTLCOLORDLG(OnCtlColorDlg)
    COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
    COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
  END_MSG_MAP()

  CSimpleClient();
  ~CSimpleClient();

  // Handler prototypes (uncomment arguments if needed):
  //	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  //	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  //	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  HBRUSH OnCtlColorDlg(CDCHandle dc, CWindow wnd);

private:
  HBRUSH m_hBkBrush;
};

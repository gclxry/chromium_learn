#pragma once
#include "atlmisc.h"

class CSimpleToolBar : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
  public CMessageFilter, public CIdleHandler
{

};
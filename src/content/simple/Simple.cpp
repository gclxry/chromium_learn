// Simple.cpp : main source file for Simple.exe
//

#include "content/public/app/content_main.h"
#include "content/simple/simple_main_delegate.h"
#include "sandbox/win/src/sandbox_types.h"
#include "content/public/app/startup_helper_win.h"

#include <windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpstrCmdLine, int nCmdShow)
{

  // «∂»Îchromium
  sandbox::SandboxInterfaceInfo sandbox_info = {0};
  content::InitializeSandboxInfo(&sandbox_info);
  content::SimpleMainDelegate delegate;
  return content::ContentMain(hInstance, &sandbox_info, &delegate);
}

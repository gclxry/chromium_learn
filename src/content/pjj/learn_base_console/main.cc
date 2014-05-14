// FeedBack.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include <Windows.h>
#include <string>
#include <iostream>

#include "base/guid.h"

int main(int argc, char* argv[])
{
  for (int ia = 0; ia < 100; ia++)
  {
    std::cout<<base::GenerateGUID()<<std::endl;
  }
  system("pause");
	return 0;
}


#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

// Windows ��� ����
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//GDI+
//#define WIN32_LEAN_AND_MEAN(���� ������ �ʴ� ������ Windows ������� ����) �����߱� ������ �۵� �� ��
//�׷��ٰ�  WIN32_LEAN_AND_MEAN ���� ���ʿ��� ��Ұ� �������� ole2.h �߰��� �ذ�
#include <ole2.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib") //GDI+ ���̺귯�� ��ũ
using namespace Gdiplus;

#include <sstream> //wstringstream
#include <string> //wstring
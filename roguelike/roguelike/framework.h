#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//GDI+
//#define WIN32_LEAN_AND_MEAN(거의 사용되지 않는 내용을 Windows 헤더에서 제외) 정의했기 때문에 작동 안 됨
//그렇다고  WIN32_LEAN_AND_MEAN 빼면 불필요한 요소가 많아지니 ole2.h 추가로 해결
#include <ole2.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib") //GDI+ 라이브러리 링크
using namespace Gdiplus;

#include <sstream> //wstringstream
#include <string> //wstring
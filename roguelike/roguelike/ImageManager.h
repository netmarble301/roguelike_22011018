#pragma once
#include <map>
using namespace std;
//GDI+
//#define WIN32_LEAN_AND_MEAN(거의 사용되지 않는 내용을 Windows 헤더에서 제외) 정의했기 때문에 작동 안 됨
//그렇다고  WIN32_LEAN_AND_MEAN 빼면 불필요한 요소가 많아지니 ole2.h 추가로 해결
#include <ole2.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib") //GDI+ 라이브러리 링크
using namespace Gdiplus;

class ImageManager
{
private:
	map<char, Image*> BlockImages;
	ImageManager();
	~ImageManager();
public:
	static ImageManager& getInstance();

	//단일 인스턴트 보장
	ImageManager(const ImageManager&) = delete;
	ImageManager& operator=(const ImageManager&) = delete;

	Image* getImage(char _key);
	map<char, Image*>::const_iterator getImageIteratorBegin();
	map<char, Image*>::const_iterator getImageIteratorEnd();

	
	//GDI+ 초기화 선언 후 호출(BlockImages 변수 초기화)
	void imageInitialize();
};


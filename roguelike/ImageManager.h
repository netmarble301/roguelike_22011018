#pragma once
#include <map>
#include <ole2.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")
using namespace Gdiplus;
using namespace std;

class ImageManager
{
private:
	map<int, Image*> blockImages;
	ImageManager();
	~ImageManager();
public:
	static ImageManager& getInstance();

	ImageManager(const ImageManager&) = delete;
	ImageManager& operator=(const ImageManager&) = delete;

	Image* getImage(int _key);

	void imageInitialize(); //BlockImages 초기화(GdiplusStartup 후에 초기화하기 위해서 생성자에 일부로 구현 안 함)
};


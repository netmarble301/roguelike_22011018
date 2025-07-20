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

	void imageInitialize(); //BlockImages �ʱ�ȭ(GdiplusStartup �Ŀ� �ʱ�ȭ�ϱ� ���ؼ� �����ڿ� �Ϻη� ���� �� ��)
};


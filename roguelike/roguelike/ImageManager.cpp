#include "ImageManager.h"

ImageManager::ImageManager()
{
	//GDI+ 초기화
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	BlockImages['0'] = Image::FromFile(L"sand_4.png");
	BlockImages['1'] = Image::FromFile(L"swamp_1.png");
	BlockImages['P'] = Image::FromFile(L"angel.png");
	BlockImages['F'] = Image::FromFile(L"enter_lair.png");
	BlockImages['M'] = Image::FromFile(L"sphinx.png"); //여러 종류의 몬스터 어떻게 할까?
}

ImageManager::~ImageManager()
{
	map<char, Image*>::iterator iter;
	for (iter = BlockImages.begin();iter != BlockImages.end();iter++)
	{
		if (iter->second)
		{
			delete iter->second; //이미지 해제
			iter->second = nullptr; //포인터 nullptr로 설정(이렇게 하는 게 좋음?)
		}
	}
	BlockImages.clear(); //map 해제

	//GDI+ 종료
	GdiplusShutdown(gdiplusToken);
}

ImageManager& ImageManager::getInstance()
{
    static ImageManager IM;
    return IM;
}

Image* ImageManager::getImage(char _key)
{
	auto it = BlockImages.find(_key); //find() : map의 멤버함수, key에 맞는 요소의 주소 반환

	if (it != BlockImages.end()) 
	{
		return it->second;
	}

	return nullptr; //찾지 못하면 nullptr 반환
}

#include "ImageManager.h"

ImageManager::ImageManager()
{
	//수정 BlockImages 초기화를 여기서 하면 안 됨, GDI+ 초기화(GdiplusStartupInput gdiplusStartupInput;) 전에 static의 ImageManager 생성자가 먼저 호출이 되어서 Image::FromFile 가 실패하게 됨
	//따로 BlockImages 초기화를 GDI+ 초기화 후 하도록 imageInitialize() 구현
}

ImageManager::~ImageManager()
{
	map<string, Image*>::iterator iter;
	for (iter = BlockImages.begin();iter != BlockImages.end();iter++)
	{
		if (iter->second)
		{
			delete iter->second; //이미지 해제
			iter->second = nullptr; //포인터 nullptr로 설정(이렇게 하는 게 좋음?)
		}
	}
	BlockImages.clear(); //map 해제
}

ImageManager& ImageManager::getInstance()
{
    static ImageManager IM;
    return IM;
}

Image* ImageManager::getImage(string _key)
{
	auto it = BlockImages.find(_key); //find() : map의 멤버함수, key에 맞는 요소의 주소 반환

	if (it != BlockImages.end()) 
	{
		return it->second;
	}

	return nullptr; //찾지 못하면 nullptr 반환
}


void ImageManager::imageInitialize()
{
	//빈 공간
	BlockImages["Empty"] = Image::FromFile(L"sand_4.png");

	//벽
	BlockImages["Wall"] = Image::FromFile(L"swamp_1.png");

	//계단(다음 층 이동)
	BlockImages["Floor"] = Image::FromFile(L"enter_lair.png");

	//플레이어
	BlockImages["Player"] = Image::FromFile(L"angel.png");

	//몬스터
	BlockImages["Sphinx"] = Image::FromFile(L"sphinx.png");
	BlockImages["Orc"] = Image::FromFile(L"orc.png");
	//...

	//아이템
	//...
}

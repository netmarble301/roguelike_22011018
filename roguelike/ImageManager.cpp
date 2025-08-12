#include "ImageManager.h"

ImageManager::ImageManager()
{
}

ImageManager::~ImageManager()
{
	map<int, Image*>::iterator iter;
	for (iter = blockImages.begin();iter != blockImages.end();iter++)
	{
		if (iter->second)
		{
			delete iter->second; //이미지 해제
			iter->second = nullptr; //포인터 nullptr 초기화
		}
	}
	blockImages.clear(); //map 해제
}

ImageManager& ImageManager::getInstance()
{
    static ImageManager imageManager;
    return imageManager;
}

Image* ImageManager::getImage(int _key)
{
	auto iter = blockImages.find(_key); //find() : map의 멤버함수, key에 맞는 요소의 주소 반환

	if (iter != blockImages.end()) 
	{
		return iter->second;
	}

	return nullptr; //찾지 못하면 nullptr 반환
}


void ImageManager::imageInitialize()
{
	//빈 공간
	blockImages[0] = Image::FromFile(L"sand_4.png");

	//벽
	blockImages[1] = Image::FromFile(L"swamp_1.png");

	//계단(다음 층 이동)
	blockImages[2] = Image::FromFile(L"enter_lair.png");

	//플레이어
	blockImages[3] = Image::FromFile(L"angel.png");
	blockImages[12] = Image::FromFile(L"angel_attacked.png");
	

	//몬스터
	blockImages[4] = Image::FromFile(L"sphinx.png");
	blockImages[5] = Image::FromFile(L"orc.png");
	blockImages[6] = Image::FromFile(L"sphinx_attacked.png");
	blockImages[7] = Image::FromFile(L"orc_attacked.png");

	//...

	//아이템
	//...

	//impact
	//blockImages[8] = Image::FromFile(L"cloud_forest_fire.png");
	blockImages[8] = Image::FromFile(L"stone_arrow_6.png");
	blockImages[9] = Image::FromFile(L"stone_arrow_0.png");
	blockImages[10] = Image::FromFile(L"stone_arrow_2.png");
	blockImages[11] = Image::FromFile(L"stone_arrow_4.png");

}

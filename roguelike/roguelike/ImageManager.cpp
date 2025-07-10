#include "ImageManager.h"

ImageManager::ImageManager()
{
	//���� BlockImages �ʱ�ȭ�� ���⼭ �ϸ� �� ��, GDI+ �ʱ�ȭ(GdiplusStartupInput gdiplusStartupInput;) ���� static�� ImageManager �����ڰ� ���� ȣ���� �Ǿ Image::FromFile �� �����ϰ� ��
	//���� BlockImages �ʱ�ȭ�� GDI+ �ʱ�ȭ �� �ϵ��� imageInitialize() ����
}

ImageManager::~ImageManager()
{
	map<char, Image*>::iterator iter;
	for (iter = BlockImages.begin();iter != BlockImages.end();iter++)
	{
		if (iter->second)
		{
			delete iter->second; //�̹��� ����
			iter->second = nullptr; //������ nullptr�� ����(�̷��� �ϴ� �� ����?)
		}
	}
	BlockImages.clear(); //map ����
}

ImageManager& ImageManager::getInstance()
{
    static ImageManager IM;
    return IM;
}

Image* ImageManager::getImage(char _key)
{
	auto it = BlockImages.find(_key); //find() : map�� ����Լ�, key�� �´� ����� �ּ� ��ȯ

	if (it != BlockImages.end()) 
	{
		return it->second;
	}

	return nullptr; //ã�� ���ϸ� nullptr ��ȯ
}


void ImageManager::imageInitialize()
{
	BlockImages['0'] = Image::FromFile(L"sand_4.png");
	BlockImages['1'] = Image::FromFile(L"swamp_1.png");
	BlockImages['P'] = Image::FromFile(L"angel.png");
	BlockImages['F'] = Image::FromFile(L"enter_lair.png");
	BlockImages['M'] = Image::FromFile(L"sphinx.png");
}

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
			delete iter->second; //�̹��� ����
			iter->second = nullptr; //������ nullptr �ʱ�ȭ
		}
	}
	blockImages.clear(); //map ����
}

ImageManager& ImageManager::getInstance()
{
    static ImageManager imageManager;
    return imageManager;
}

Image* ImageManager::getImage(int _key)
{
	auto iter = blockImages.find(_key); //find() : map�� ����Լ�, key�� �´� ����� �ּ� ��ȯ

	if (iter != blockImages.end()) 
	{
		return iter->second;
	}

	return nullptr; //ã�� ���ϸ� nullptr ��ȯ
}


void ImageManager::imageInitialize()
{
	//�� ����
	blockImages[0] = Image::FromFile(L"sand_4.png");

	//��
	blockImages[1] = Image::FromFile(L"swamp_1.png");

	//���(���� �� �̵�)
	blockImages[2] = Image::FromFile(L"enter_lair.png");

	//�÷��̾�
	blockImages[3] = Image::FromFile(L"angel.png");

	//����
	blockImages[4] = Image::FromFile(L"sphinx.png");
	blockImages[5] = Image::FromFile(L"orc.png");
	//...

	//������
	//...
}

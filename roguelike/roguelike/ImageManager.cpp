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
	blockImages[12] = Image::FromFile(L"angel_attacked.png");
	

	//����
	blockImages[4] = Image::FromFile(L"sphinx.png");
	blockImages[5] = Image::FromFile(L"orc.png");
	blockImages[6] = Image::FromFile(L"sphinx_attacked.png");
	blockImages[7] = Image::FromFile(L"orc_attacked.png");

	//...

	//������
	//...

	//impact
	//blockImages[8] = Image::FromFile(L"cloud_forest_fire.png");
	blockImages[8] = Image::FromFile(L"stone_arrow_6.png");
	blockImages[9] = Image::FromFile(L"stone_arrow_0.png");
	blockImages[10] = Image::FromFile(L"stone_arrow_2.png");
	blockImages[11] = Image::FromFile(L"stone_arrow_4.png");

}

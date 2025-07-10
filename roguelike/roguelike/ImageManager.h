#pragma once
#include <map>
using namespace std;
//GDI+
//#define WIN32_LEAN_AND_MEAN(���� ������ �ʴ� ������ Windows ������� ����) �����߱� ������ �۵� �� ��
//�׷��ٰ�  WIN32_LEAN_AND_MEAN ���� ���ʿ��� ��Ұ� �������� ole2.h �߰��� �ذ�
#include <ole2.h>
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib") //GDI+ ���̺귯�� ��ũ
using namespace Gdiplus;

class ImageManager
{
private:
	map<char, Image*> BlockImages;
	ImageManager();
	~ImageManager();
public:
	static ImageManager& getInstance();

	//���� �ν���Ʈ ����
	ImageManager(const ImageManager&) = delete;
	ImageManager& operator=(const ImageManager&) = delete;

	Image* getImage(char _key);
	map<char, Image*>::const_iterator getImageIteratorBegin();
	map<char, Image*>::const_iterator getImageIteratorEnd();

	
	//GDI+ �ʱ�ȭ ���� �� ȣ��(BlockImages ���� �ʱ�ȭ)
	void imageInitialize();
};


#include <stdio.h>
#include<iostream>
#include<graphics.h>

#include"tools.hpp"
#include<easyx.h>

//**********************************��Ƶ���ȣ�0:30:30
//*******************************��C����/C++������Ϸ��Ŀ�̡̳��ɻ���ս���ǳ��ʺ�С��������ϰ�� ����׼�ս��� 30:29�� https://www.bilibili.com/video/BV1yB4y197n9/?share_source=copy_web&vd_source=364ffb9d4a7b441a9c99ebdebd35a326&t=1829

IMAGE img_bk;
IMAGE img_gamer[2];//�ɻ�ͼƬ



//������һ��Plane�µĶ������ֽ�gamer
struct Plane {
	int x;
	int y;
	bool isDie;
}gamer;

void loadResource() {
	loadimage(&img_bk, "asset/image/background.png");
	//loadimage�ĵ�һ��������ָ�룬Ҫ����img_bk�ĵ�ַ

	loadimage(img_gamer + 0, "asset/image/hero1.png");
	loadimage(img_gamer + 1, "asset/image/hero2.png");
	//img_gamer+0 =�ȼ���= &img_gamer[0]  
	// ��������������׵�ַ��+0�ǵ�һ��Ԫ�ص�ַ��+1���ǵڶ���Ԫ��
	
}

//��ʼ���ɻ�
void plane_init(Plane* pthis, int x, int y) {
	pthis->x = x;
	pthis->y = y;
	pthis->isDie = false;

}

//
//���Ʒɻ�
void plane_draw(Plane* pthis) {
	drawImg(pthis->x, pthis->y, img_gamer + 0);
}


//**********************���е����ݳ�ʼ��
void init() {
	loadResource();

	int planeW = img_gamer[0].getwidth();
	int planeH = img_gamer[0].getheight();

	//����plane�Ĵ���ע��ɻ���λ�������Ͻ�Ϊ׼��
	plane_init(&gamer, (getwidth() - planeW) / 2, getheight() - planeH);


}


//*********************���ƽ���
void draw() {
		//���ͼƬ
	putimage(0, 0, &img_bk);

	//����ɻ�
	//putimage(0, 0, img_gamer + 0);easyX��֧��͸����ͼ
	plane_draw(&gamer);
}


int main() {
	initgraph(480,850);

	init();

	draw();
	
	



	getchar();//�����������

	return 0;
}
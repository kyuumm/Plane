#include <stdio.h>
#include<iostream>
#include<graphics.h>

#include"tools.hpp"
#include<easyx.h>

#include<time.h>

#define BULLET_NUM 30//�ӵ�����
//**********************************��Ƶ���ȣ�0:30:30
//*******************************

IMAGE img_bk;
IMAGE img_gamer[2];//�ɻ�ͼƬ
IMAGE img_bullet[2];//�ӵ�ͼƬ

//�Զ��嶨ʱ��
bool Timer(int ms, int id) {
	static int start[5];
	int end = clock();

	if (end-start[id]>=ms)
	{
		start[id] = end;
		return true;
	}
	return false;
}

//������һ��Plane�µĶ������ֽ�gamer
struct Plane {
	int x;
	int y;
	bool isDie;

	int frame = 0;//��ǰ֡
}gamer;

struct Bullet {
	int x;
	int y;
	bool isDie;
	int frame = 0;//��ǰ֡
}bullet[BULLET_NUM] = { 0 };

void createBullet() {
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].isDie) {
			bullet[i].x = gamer.x + img_gamer->getwidth() / 2;
			bullet[i].y = gamer.y;

			bullet[i].isDie = false;
			break;
		}
	}
}

//�ӵ��ƶ�
void moveBullet() {
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (!bullet[i].isDie) {
			bullet[i].y -= 5;
		//�ж��ӵ��Ƿ����
			if (bullet[i].y<0)
			{
				bullet[i].isDie = true;
			}

		}

	}
}


void loadResource() {
	loadimage(&img_bk, "asset/image/background.png");
	//loadimage�ĵ�һ��������ָ�룬Ҫ����img_bk�ĵ�ַ

	loadimage(img_gamer + 0, "asset/image/hero1.png");
	loadimage(img_gamer + 1, "asset/image/hero2.png");
	//img_gamer+0 =�ȼ���= &img_gamer[0]  
	// ��������������׵�ַ��+0�ǵ�һ��Ԫ�ص�ַ��+1���ǵڶ���Ԫ��
	
	//�����ӵ�
	loadimage(img_bullet + 0, "asset/image/bullet1.png");
	loadimage(img_bullet + 1, "asset/image/bullet2.png");
}

//��ʼ���ɻ�
void plane_init(Plane* pthis, int x, int y) {
	pthis->x = x;
	pthis->y = y;
	pthis->isDie = false;

	pthis->frame = 0;
}

//

//���Ʒɻ�
void plane_draw(Plane* pthis) {
	drawImg(pthis->x, pthis->y, img_gamer + pthis->frame);
	pthis->frame=(pthis->frame+1)%2;

}
//�ƶ��ɻ�  
void plane_move(Plane* pthis) {  

	int speed = 3;

   //��ȡ���̵İ���  
   if (GetAsyncKeyState(VK_UP)&&pthis->y>0) {  
       pthis->y-=speed;  
   }  

   if (GetAsyncKeyState(VK_DOWN)&&img_bk.getheight()-img_gamer->getheight()-pthis->y>0) {  
       pthis->y+=speed;  
   }  

   if (GetAsyncKeyState(VK_LEFT)&&(pthis->x+img_gamer->getwidth()/2>0)) {  
       pthis->x-=speed;  
   }  

   if (GetAsyncKeyState(VK_RIGHT)&&(pthis->x+img_gamer->getwidth()/2)<img_bk.getwidth()) {  
       pthis->x+=speed;  
   }  

   if (GetAsyncKeyState(VK_SPACE)&&Timer(100,0)) {
	   createBullet();
   }
}

//**********************���е����ݳ�ʼ��
void init() {
	loadResource();

	int planeW = img_gamer[0].getwidth();
	int planeH = img_gamer[0].getheight();

	//����plane�Ĵ���ע��ɻ���λ�������Ͻ�Ϊ׼��
	plane_init(&gamer, (getwidth() - planeW) / 2, getheight() - planeH);

	//��ʼ���ӵ�
	for (int i = 0;i < BULLET_NUM ; i++) {
		bullet[i].isDie = true;
	}

}

//*********************���е���Դ�ͷ�

//*********************���ƽ���
void draw() {
		//���ͼƬ
	putimage(0, 0, &img_bk);

	//����ɻ�
	//putimage(0, 0, img_gamer + 0);easyX��֧��͸����ͼ
	plane_draw(&gamer);

	//����ӵ�
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (!bullet[i].isDie)
		{
			drawImg(bullet[i].x, bullet[i].y, img_bullet + 0);
		}

	}
}


int main() {
	initgraph(480,850);
	init();



	while(true){
		int startTime = clock();//��ȡ����ִ�е����ú��������ĺ�����
		draw();

		plane_move(&gamer);
		moveBullet();

		int frameTime = clock() - startTime;//��ȡ����ִ�е����ú��������ĺ�����

		//һ֡Ӧ��ִ�е�ʱ����ڵ�ǰִ֡�е�ʱ��
		if (1000 / 60 - frameTime > 0) {
			Sleep(1000 / 60 - frameTime);
		}
	}


	
	



	getchar();//�����������

	return 0;
}
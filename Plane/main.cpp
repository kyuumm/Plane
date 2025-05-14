#include <stdio.h>
#include<iostream>
#include<graphics.h>

#include"tools.hpp"
#include<easyx.h>

#include<time.h>

#define BULLET_NUM 30//子弹数量
//**********************************视频进度：0:30:30
//*******************************

IMAGE img_bk;
IMAGE img_gamer[2];//飞机图片
IMAGE img_bullet[2];//子弹图片

//自定义定时器
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

//创建了一个Plane下的对象，名字叫gamer
struct Plane {
	int x;
	int y;
	bool isDie;

	int frame = 0;//当前帧
}gamer;

struct Bullet {
	int x;
	int y;
	bool isDie;
	int frame = 0;//当前帧
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

//子弹移动
void moveBullet() {
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (!bullet[i].isDie) {
			bullet[i].y -= 5;
		//判断子弹是否出界
			if (bullet[i].y<0)
			{
				bullet[i].isDie = true;
			}

		}

	}
}


void loadResource() {
	loadimage(&img_bk, "asset/image/background.png");
	//loadimage的第一个参数是指针，要传入img_bk的地址

	loadimage(img_gamer + 0, "asset/image/hero1.png");
	loadimage(img_gamer + 1, "asset/image/hero2.png");
	//img_gamer+0 =等价于= &img_gamer[0]  
	// 数组名是数组的首地址，+0是第一个元素地址，+1就是第二个元素
	
	//加载子弹
	loadimage(img_bullet + 0, "asset/image/bullet1.png");
	loadimage(img_bullet + 1, "asset/image/bullet2.png");
}

//初始化飞机
void plane_init(Plane* pthis, int x, int y) {
	pthis->x = x;
	pthis->y = y;
	pthis->isDie = false;

	pthis->frame = 0;
}

//

//绘制飞机
void plane_draw(Plane* pthis) {
	drawImg(pthis->x, pthis->y, img_gamer + pthis->frame);
	pthis->frame=(pthis->frame+1)%2;

}
//移动飞机  
void plane_move(Plane* pthis) {  

	int speed = 3;

   //获取键盘的按键  
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

//**********************所有的数据初始化
void init() {
	loadResource();

	int planeW = img_gamer[0].getwidth();
	int planeH = img_gamer[0].getheight();

	//居中plane的处理：注意飞机的位置以左上角为准心
	plane_init(&gamer, (getwidth() - planeW) / 2, getheight() - planeH);

	//初始化子弹
	for (int i = 0;i < BULLET_NUM ; i++) {
		bullet[i].isDie = true;
	}

}

//*********************所有的资源释放

//*********************绘制界面
void draw() {
		//输出图片
	putimage(0, 0, &img_bk);

	//输出飞机
	//putimage(0, 0, img_gamer + 0);easyX不支持透明贴图
	plane_draw(&gamer);

	//输出子弹
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
		int startTime = clock();//获取程序执行到调用函数经过的毫秒数
		draw();

		plane_move(&gamer);
		moveBullet();

		int frameTime = clock() - startTime;//获取程序执行到调用函数经过的毫秒数

		//一帧应该执行的时间大于当前帧执行的时间
		if (1000 / 60 - frameTime > 0) {
			Sleep(1000 / 60 - frameTime);
		}
	}


	
	



	getchar();//按任意键结束

	return 0;
}
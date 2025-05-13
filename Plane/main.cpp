#include <stdio.h>
#include<iostream>
#include<graphics.h>

#include"tools.hpp"
#include<easyx.h>

//**********************************视频进度：0:30:30
//*******************************【C语言/C++经典游戏项目教程—飞机大战，非常适合小白入门练习】 【精准空降到 30:29】 https://www.bilibili.com/video/BV1yB4y197n9/?share_source=copy_web&vd_source=364ffb9d4a7b441a9c99ebdebd35a326&t=1829

IMAGE img_bk;
IMAGE img_gamer[2];//飞机图片



//创建了一个Plane下的对象，名字叫gamer
struct Plane {
	int x;
	int y;
	bool isDie;
}gamer;

void loadResource() {
	loadimage(&img_bk, "asset/image/background.png");
	//loadimage的第一个参数是指针，要传入img_bk的地址

	loadimage(img_gamer + 0, "asset/image/hero1.png");
	loadimage(img_gamer + 1, "asset/image/hero2.png");
	//img_gamer+0 =等价于= &img_gamer[0]  
	// 数组名是数组的首地址，+0是第一个元素地址，+1就是第二个元素
	
}

//初始化飞机
void plane_init(Plane* pthis, int x, int y) {
	pthis->x = x;
	pthis->y = y;
	pthis->isDie = false;

}

//
//绘制飞机
void plane_draw(Plane* pthis) {
	drawImg(pthis->x, pthis->y, img_gamer + 0);
}


//**********************所有的数据初始化
void init() {
	loadResource();

	int planeW = img_gamer[0].getwidth();
	int planeH = img_gamer[0].getheight();

	//居中plane的处理：注意飞机的位置以左上角为准心
	plane_init(&gamer, (getwidth() - planeW) / 2, getheight() - planeH);


}


//*********************绘制界面
void draw() {
		//输出图片
	putimage(0, 0, &img_bk);

	//输出飞机
	//putimage(0, 0, img_gamer + 0);easyX不支持透明贴图
	plane_draw(&gamer);
}


int main() {
	initgraph(480,850);

	init();

	draw();
	
	



	getchar();//按任意键结束

	return 0;
}
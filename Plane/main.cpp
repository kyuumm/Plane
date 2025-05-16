#include <stdio.h>
#include<iostream>
#include<graphics.h>

#include"tools.hpp"
#include<easyx.h>

#include<time.h>


#define BULLET_NUM 30//子弹数量
#define ENEMY_NUM 7//敌机数量

#define ENEMY_SPEED 3
#define ENEMY_BULLET_NUM 10

#define GAMER_HEART 3
//**********************************视频进度：0:30:30
//*******************************

IMAGE img_bk;
IMAGE img_gamer[2];//飞机图片
IMAGE img_bullet[2];//子弹图片
IMAGE img_enemy[2];//敌机图片

IMAGE img_btn_finish;
IMAGE img_bk_mask;

IMAGE img_btn_die;

IMAGE img_heart;





bool isPaused = false;
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

	int gamerHeart=GAMER_HEART;
	int score = 0;

	bool isDie;

	int frame = 0;//当前帧
}gamer;

Plane enemy[ENEMY_NUM]{

};



struct Bullet {
	int x;
	int y;
	bool isDie;
	int frame = 0;//当前帧
}bullet[BULLET_NUM] = { 0 };

Bullet enemy_bullets[ENEMY_BULLET_NUM];

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

//敌机
void createEnemy() {
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].isDie) {
			enemy[i].x = rand() % getwidth();
			enemy[i].y = -img_enemy->getheight();

			enemy[i].isDie = false;
			break;
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

	//加载敌机
	loadimage(img_enemy + 0, "asset/image/enemy0.png");
	loadimage(img_enemy + 1, "asset/image/enemy1.png");

	loadimage(&img_btn_finish, "asset/image/btn_finish.png");
	loadimage(&img_bk_mask, "asset/image/background_mask.png");

	loadimage(&img_btn_die, "asset/image/btn_die.png");

	loadimage(&img_heart, "asset/image/heart.png");
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

	if (GetAsyncKeyState(VK_ESCAPE)) {
		isPaused = !isPaused;
		Sleep(200);
	}

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

//敌机移动
void enemy_move() {
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].isDie) {

			enemy[i].y += ENEMY_SPEED;

			if (enemy[i].y > getheight()) {
				enemy[i].isDie = true;
			}
		}
	}
}

//敌机发射子弹
void enemy_create_bullet() {
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].isDie && rand() % 20 == 0) {
			for (int j = 0; j < ENEMY_BULLET_NUM; j++)
			{
				if (enemy_bullets[j].isDie) {
					enemy_bullets[j].x = enemy[i].x + img_enemy->getwidth() / 2;
					enemy_bullets[j].y = enemy[i].y + img_enemy->getheight();
					enemy_bullets[j].isDie = false;
					break;
				}
			}
		}
	}
}

//敌机子弹移动//
void moveEnemyBullet() {
	for (int i = 0; i < ENEMY_BULLET_NUM; i++)
	{
		if (!enemy_bullets[i].isDie) {
			enemy_bullets[i].y += 5;

			if (enemy_bullets[i].y>getheight())
			{
				enemy_bullets[i].isDie = true;
			}
		}
	}
}

//碰撞检测
void collisionDetection(Plane *pthis) {
	//敌机打我
	for (int i = 0; i < ENEMY_BULLET_NUM; i++)
	{
		if (!enemy_bullets[i].isDie){
			int bx = enemy_bullets[i].x;
			int by = enemy_bullets[i].y;
			
			if (bx>=gamer.x&&bx<=gamer.x+img_gamer->getwidth()&&
				by>=gamer.y&&by<=gamer.y+img_gamer->getheight())
			{
				enemy_bullets[i].isDie = true;

				pthis->gamerHeart--;
				if (pthis->gamerHeart<=0)
				{
					gamer.isDie = true;
					isPaused = true;
				}


			}
		}
	}
	//我打敌机
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (!bullet[i].isDie) {
			int ax = bullet[i].x;
			int ay = bullet[i].y;

			for (int j = 0; j < ENEMY_NUM; j++)
			{
				if (ax >= enemy[j].x && ax <= enemy[j].x + img_enemy->getwidth() &&
					ay >= enemy[j].y && ay <= enemy[j].y + img_enemy->getheight())
				{
					enemy_bullets[j].isDie = true;
					enemy[j].isDie = true;

					bullet[i].isDie = true;

					pthis->score++;

				}
			}

		}
		
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
	//初始化敌机
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].isDie = true;
	}
	//初始化敌机子弹
	for (int i = 0; i < ENEMY_BULLET_NUM; i++)
	{
		enemy_bullets[i].isDie = true;
	}
}

//*********************所有的资源释放

//*********************绘制界面
void draw(Plane*pthis) {
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

	//输出敌机
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].isDie) {
			drawImg(enemy[i].x, enemy[i].y, img_enemy + 0);
		}
	}
	//输出敌机子弹
	for (int i = 0; i < ENEMY_BULLET_NUM; i++)
	{
		if (!enemy_bullets[i].isDie) {
			drawImg(enemy_bullets[i].x, enemy_bullets[i].y, img_bullet + 1);
		}
	}
	//输出生命值
	for (int i = 0; i < pthis->gamerHeart; i++)
	{
		int heartX = 10 + i * (img_heart.getwidth() + 5);
		int heartY = getheight() - img_heart.getheight() - 10;
		drawImg(heartX, heartY, &img_heart);
	}

	char scoreText[50];
	sprintf_s(scoreText, "Score:%d", pthis->score);
	outtextxy(10, 10, scoreText);

}


int main() {


	initgraph(480,850);
	init();




	
	while(true){
		

		if (GetAsyncKeyState(VK_ESCAPE)) {
			isPaused = !isPaused;
			Sleep(200);
		}
		//暂停


		if (!isPaused&&!gamer.isDie) {
			int startTime = clock();//获取程序执行到调用函数经过的毫秒数
			draw(&gamer);

			plane_move(&gamer);
			moveBullet();

			collisionDetection(&gamer);

			if (Timer(400, 0)) {
				createEnemy();
			}
	
			enemy_move();
			enemy_create_bullet();
			moveEnemyBullet();

			int frameTime = clock() - startTime;//获取程序执行到调用函数经过的毫秒数

			//一帧应该执行的时间大于当前帧执行的时间
			if (1000 / 60 - frameTime > 0) {
				Sleep(1000 / 60 - frameTime);
			}
		}

		if (isPaused&&gamer.isDie) {



			drawImg((getwidth() - img_btn_finish.getwidth()) / 2, (getheight() - img_btn_finish.getheight()) / 2, &img_btn_die);
			
		}
		if (isPaused&&!gamer.isDie) {

			

			drawImg((getwidth() - img_btn_finish.getwidth()) / 2, (getheight() - img_btn_finish.getheight()) / 2, &img_btn_finish);
			
		}
	
	}


	
	



	getchar();//按任意键结束

	return 0;
}
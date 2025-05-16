#include <stdio.h>
#include<iostream>
#include<graphics.h>

#include"tools.hpp"
#include<easyx.h>

#include<time.h>


#define BULLET_NUM 30//�ӵ�����
#define ENEMY_NUM 7//�л�����

#define ENEMY_SPEED 3
#define ENEMY_BULLET_NUM 10

#define GAMER_HEART 3
//**********************************��Ƶ���ȣ�0:30:30
//*******************************

IMAGE img_bk;
IMAGE img_gamer[2];//�ɻ�ͼƬ
IMAGE img_bullet[2];//�ӵ�ͼƬ
IMAGE img_enemy[2];//�л�ͼƬ

IMAGE img_btn_finish;
IMAGE img_bk_mask;

IMAGE img_btn_die;

IMAGE img_heart;





bool isPaused = false;
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

	int gamerHeart=GAMER_HEART;
	int score = 0;

	bool isDie;

	int frame = 0;//��ǰ֡
}gamer;

Plane enemy[ENEMY_NUM]{

};



struct Bullet {
	int x;
	int y;
	bool isDie;
	int frame = 0;//��ǰ֡
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

//�л�
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
	//loadimage�ĵ�һ��������ָ�룬Ҫ����img_bk�ĵ�ַ

	loadimage(img_gamer + 0, "asset/image/hero1.png");
	loadimage(img_gamer + 1, "asset/image/hero2.png");
	//img_gamer+0 =�ȼ���= &img_gamer[0]  
	// ��������������׵�ַ��+0�ǵ�һ��Ԫ�ص�ַ��+1���ǵڶ���Ԫ��
	
	//�����ӵ�
	loadimage(img_bullet + 0, "asset/image/bullet1.png");
	loadimage(img_bullet + 1, "asset/image/bullet2.png");

	//���صл�
	loadimage(img_enemy + 0, "asset/image/enemy0.png");
	loadimage(img_enemy + 1, "asset/image/enemy1.png");

	loadimage(&img_btn_finish, "asset/image/btn_finish.png");
	loadimage(&img_bk_mask, "asset/image/background_mask.png");

	loadimage(&img_btn_die, "asset/image/btn_die.png");

	loadimage(&img_heart, "asset/image/heart.png");
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

	if (GetAsyncKeyState(VK_ESCAPE)) {
		isPaused = !isPaused;
		Sleep(200);
	}

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

//�л��ƶ�
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

//�л������ӵ�
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

//�л��ӵ��ƶ�//
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

//��ײ���
void collisionDetection(Plane *pthis) {
	//�л�����
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
	//�Ҵ�л�
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
	//��ʼ���л�
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].isDie = true;
	}
	//��ʼ���л��ӵ�
	for (int i = 0; i < ENEMY_BULLET_NUM; i++)
	{
		enemy_bullets[i].isDie = true;
	}
}

//*********************���е���Դ�ͷ�

//*********************���ƽ���
void draw(Plane*pthis) {
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

	//����л�
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].isDie) {
			drawImg(enemy[i].x, enemy[i].y, img_enemy + 0);
		}
	}
	//����л��ӵ�
	for (int i = 0; i < ENEMY_BULLET_NUM; i++)
	{
		if (!enemy_bullets[i].isDie) {
			drawImg(enemy_bullets[i].x, enemy_bullets[i].y, img_bullet + 1);
		}
	}
	//�������ֵ
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
		//��ͣ


		if (!isPaused&&!gamer.isDie) {
			int startTime = clock();//��ȡ����ִ�е����ú��������ĺ�����
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

			int frameTime = clock() - startTime;//��ȡ����ִ�е����ú��������ĺ�����

			//һ֡Ӧ��ִ�е�ʱ����ڵ�ǰִ֡�е�ʱ��
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


	
	



	getchar();//�����������

	return 0;
}
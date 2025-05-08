#include <stdio.h>
#include"tools.hpp"
#include<easyx.h>
int main() {
	initgraph(480, 852);

	IMAGE img_bk;

	loadimage(&img_bk, "asset/image/background.png");
	putimage(0, 0, &img_bk);
	//test
	getchar();
	return 0;
}
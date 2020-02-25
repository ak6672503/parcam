#include "GMap.h"

int GMap::LD = 36; //墙的宽度
int GMap::PD = 3;  //豆子的直径

//敌我双方出现位置没有豆子出现
void GMap::InitOP() {
	peaMapData[E_ROW][E_ARRAY] = false;  //地方位置没有豆子
	peaMapData[P_ROW][P_ARRAY] = false;  //玩家位置没有豆子
}
//画墙壁
void GMap::DrawMap(HDC &memDC) {
	HBRUSH hBrush = CreateSolidBrush(color);
	for (int i = 0; i < MAPLENTH; i++) {
		for (int j = 0; j < MAPLENTH; j++) {
		//绘制墙壁
			if (!mapData[i][j]) {
				RECT rect;
				rect.left = j * LD;
				rect.top = i * LD;
				rect.right = (j + 1) * LD;
				rect.bottom = (i + 1) * LD;
				FillRect(memDC, &rect, hBrush); //填充矩形区域，模拟墙体
			}
		}
	}
	DeleteObject(hBrush); //删除画刷对象
}
//画豆子函数
void GMap::DrawPeas(HDC& hdc) {

	for (int i = 0; i < MAPLENTH; i++) {
		for (int j = 0; j < MAPLENTH; j++) {
			if (peaMapData[i][j]) {
				Ellipse(hdc, (LD / 2 - PD) + j * LD,
					(LD / 2 - PD) + i * LD,
					(LD / 2 + PD) + j * LD,
					(LD / 2 + PD) + i * LD);
			}
		}
	}
	//游戏隐藏后门
	if (GetAsyncKeyState('B') & 0x8000) {
		MessageBoxA(NULL, "发现了秘籍","", MB_OK);
		for (int i = 0; i < MAPLENTH; i++) {
			for (int j = 0; i < MAPLENTH; j++) {
				peaMapData[i][j] = false;

}


		}
	}
}

GMap::~GMap() {}

//stage_1成员的定义：
#define A true //true表示豆子
#define B false //false表示墙壁

bool Stage_1::initData[MAPLENTH][MAPLENTH] = {
B,B,B,B,B,B,B,B,A,B,B,B,B,B,B,B,B,B,B, // 0
B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B, // 1
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//2
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//3
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//4
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//5
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//6
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//7
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//8 
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//9
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//10
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//11
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//12
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//13
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//14
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//15
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//16
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//17
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B//18
};

#undef A
#undef B
Stage_1::Stage_1() {
	color = RGB(140, 240, 240);
	for (int i = 0; i < MAPLENTH; i++) {
		for (int j = 0; j < MAPLENTH; j++) {
			this->mapData[i][j] = this->initData[i][j];
			this->peaMapData[i][j] = this->initData[i][j];
		}
	}
	//敌我双方出现的位置没有豆子出现
	this->InitOP();
}


//stage_2成员的定义：
#define A true //true表示豆子
#define B false //false表示墙壁

bool Stage_2::initData[MAPLENTH][MAPLENTH] = {
B,B,B,B,B,B,B,B,A,B,B,B,B,B,B,B,B,B,B, // 0
B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B, // 1
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//2
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//3
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//4
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//5
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//6
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//7
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//8 
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//9
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//10
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//11
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//12
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//13
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//14
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//15
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//16
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//17
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B//18
};

#undef A
#undef B
Stage_2::Stage_2() {
	color = RGB(240,140,140);
	for (int i = 0; i < MAPLENTH; i++) {
		for (int j = 0; j < MAPLENTH; j++) {
			this->mapData[i][j] = this->initData[i][j];
			this->peaMapData[i][j] = this->initData[i][j];
		}
	}
	//敌我双方出现的位置没有豆子出现
	this->InitOP();
}


//stage_3成员的定义：
#define A true //true表示豆子
#define B false //false表示墙壁

bool Stage_3::initData[MAPLENTH][MAPLENTH] = {
B,B,B,B,B,B,B,B,A,B,B,B,B,B,B,B,B,B,B, // 0
B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B, // 1
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//2
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//3
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//4
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//5
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//6
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//7
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//8 
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//9
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//10
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//11
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//12
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//13
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//14
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//15
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B,//16
B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//17
B,A,A,A,A,A,A,A,A,A,B,A,B,A,A,A,A,A,B//18
};

#undef A
#undef B
Stage_3::Stage_3() {
	color = RGB(100,44,100);
	for (int i = 0; i < MAPLENTH; i++) {
		for (int j = 0; j < MAPLENTH; j++) {
			this->mapData[i][j] = this->initData[i][j];
			this->peaMapData[i][j] = this->initData[i][j];
		}
	}
	//敌我双方出现的位置没有豆子出现
	this->InitOP();
}
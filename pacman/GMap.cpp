#include "GMap.h"

int GMap::LD = 36; //ǽ�Ŀ��
int GMap::PD = 3;  //���ӵ�ֱ��

//����˫������λ��û�ж��ӳ���
void GMap::InitOP() {
	peaMapData[E_ROW][E_ARRAY] = false;  //�ط�λ��û�ж���
	peaMapData[P_ROW][P_ARRAY] = false;  //���λ��û�ж���
}
//��ǽ��
void GMap::DrawMap(HDC &memDC) {
	HBRUSH hBrush = CreateSolidBrush(color);
	for (int i = 0; i < MAPLENTH; i++) {
		for (int j = 0; j < MAPLENTH; j++) {
		//����ǽ��
			if (!mapData[i][j]) {
				RECT rect;
				rect.left = j * LD;
				rect.top = i * LD;
				rect.right = (j + 1) * LD;
				rect.bottom = (i + 1) * LD;
				FillRect(memDC, &rect, hBrush); //����������ģ��ǽ��
			}
		}
	}
	DeleteObject(hBrush); //ɾ����ˢ����
}
//�����Ӻ���
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
	//��Ϸ���غ���
	if (GetAsyncKeyState('B') & 0x8000) {
		MessageBoxA(NULL, "�������ؼ�","", MB_OK);
		for (int i = 0; i < MAPLENTH; i++) {
			for (int j = 0; i < MAPLENTH; j++) {
				peaMapData[i][j] = false;

}


		}
	}
}

GMap::~GMap() {}

//stage_1��Ա�Ķ��壺
#define A true //true��ʾ����
#define B false //false��ʾǽ��

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
	//����˫�����ֵ�λ��û�ж��ӳ���
	this->InitOP();
}


//stage_2��Ա�Ķ��壺
#define A true //true��ʾ����
#define B false //false��ʾǽ��

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
	//����˫�����ֵ�λ��û�ж��ӳ���
	this->InitOP();
}


//stage_3��Ա�Ķ��壺
#define A true //true��ʾ����
#define B false //false��ʾǽ��

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
	//����˫�����ֵ�λ��û�ж��ӳ���
	this->InitOP();
}
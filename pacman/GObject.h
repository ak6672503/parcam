#pragma once
#include"framework.h"
#include<time.h>
#include"GMap.h"

#define PLAYERSPEED 6 //����ٶ�
#define ENERMYSPEED 4 //�����ٶ�
#define LEGCOUNTS 5 //�����ȵ�����
#define DISTANCE 10 //ͼ�η�Χ
#define BLUE_ALERT 8 //��ɫ���䷶Χ
#define D_OFFSET 2 //��Χ
#define RD (DISTANCE + D_OFFSET) //��ͼ��Χ


//����ö��
enum  TWARDS
{
	//����ö��
	UP,  
	DOWN,
	LEFT,
	RIGHT,
	OVER,  //��Ϸ����
};

class GObject //�����ࣺ����͵��˵ĸ���
{
public:
	GObject(int Row, int Array) {
		m_nFrame = 1; //֡��
		pStage = NULL;  //��ǰ�ؿ�
		this->m_nRow = Row; //��
		this->m_nArray = Array; //����

		//����λ��
		this->m_ptCenter.y = m_nRow * pStage->LD + pStage->LD / 2;
		this->m_ptCenter.x = m_nArray * pStage->LD + pStage->LD / 2;
	}
	void SetPosition(int Row, int Array); //����λ��
	void DrawBlank(HDC& hdc); //���հ�
	void virtual Draw(HDC& hdc) = 0; //���ƶ���
	void virtual action() = 0; //���ݱ���ı���
	int GetRow();
	int GetArray();

	static GMap* pStage;  //ָ���ͼ��ָ�� ����Ϊ��̬��ʹ������������ܹ�ʹ����ͬ�ĵ�ͼ



protected:
	int m_nX;
	int m_nY;
	TWARDS m_cmd; //ָ���
	POINT m_ptCenter; //��������
	int m_nRow; //�߼�������
	int m_nArray;//�߼�������
	int m_nSpeed; //�ٶ�
	TWARDS m_dir;//����
	int m_nFrame; //֡��
	bool Achive(); // �ж������Ƿ񵽴��߼�����λ��
	bool Collision(); //�߼���ײ���
	int PtTransform(int k);  //��ʵ������ת��Ϊ�߼���Ϊ
	virtual void AchiveCtrl();  //�����߼�����������
};

//��Ҷ���
class PacMan : public GObject {  //��Ҷ���
protected:
	virtual void AchiveCtrl(); //��д�麯��

public:
	POINT GetPos();
	bool IsOver(); //��Ϸ�Ƿ����
	bool IsWin(); //����Ƿ�Ӯ����Ϸ
	void Draw(HDC& hdc); //��������Լ�
	void SetTwCommand(TWARDS command); //���������һ��ָ��
	PacMan(int x, int y) :GObject(x, y) {//���캯���������¶���ʱ����
		this->m_nSpeed = PLAYERSPEED; //��������ٶ�
		m_cmd = m_dir = LEFT;  //���ó���
	}
	void action();   //��ҵĶ�������
	void SetOver();  //������Ϸ��������
};

//�о�����
class Enermy : public GObject
{
protected:
	void Catch();  //�Ƿ�ץס���

	void virtual MakeDecision(bool b) = 0;  //AIʵ�� ȷ������
	COLORREF color;
public:
	static std::shared_ptr<PacMan> player;

	void virtual Draw(HDC& hdc); //��������Լ�
	//���캯��
	Enermy(int x, int y) :GObject(x, y) {
		this->m_nSpeed = ENERMYSPEED; //�����ٶ�
		m_dir = LEFT; //���ó���
		m_cmd = UP;  //�����ƶ�����
	}
	void virtual action();   //������Ϊ
};

//3�ֵ���
class RedOne :public Enermy //����ƶ�
{
protected:
	void virtual MakeDecision(bool b);
public:
	void Draw(HDC& hdc);
	RedOne(int x, int y) :Enermy(x, y) {
		color = RGB(255, 0, 0);
	}
};

//������
class BlueOne :public RedOne
{
protected:
	void virtual MakeDecision(bool b);
public:
	void Draw(HDC& hdc);
	BlueOne(int x, int y) : RedOne(x, y) {
	}

};

//������
class YellowOne :public RedOne {
protected:
	void virtual MakeDecision(bool b);
public:
	void Draw(HDC& hdc);
	YellowOne(int x, int y) :RedOne(x, y) {
		color = RGB(200, 200, 100);
	}

};
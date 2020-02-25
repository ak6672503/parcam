#include "GObject.h"
#include "framework.h"

//GOject成员定义
GMap* GObject::pStage = NULL;

int GObject::GetRow() {
	return m_nRow;
}

int GObject::GetArray() {
	return m_nArray;
}

int GObject::PtTransform(int k) {
	return (k - (pStage->LD) / 2) / pStage->LD;
}
//到达逻辑点后更新数据
void GObject::AchiveCtrl()
{
	if (Achive()) { //如果达到逻辑坐标
		m_nArray = PtTransform(m_ptCenter.x); ////更新列
		m_nRow = PtTransform(m_ptCenter.y); //更新行
	}
}

//判断物体是否到达逻辑坐标位置
bool GObject::Achive() {
	int n = (m_ptCenter.x - pStage->LD / 2) % pStage->LD;
	int k = (m_ptCenter.y - pStage->LD / 2) % pStage->LD; //计算Y坐标的余数
	bool l = (n == 0 && k == 0); //如果两个余数为0，说明到达中心位置
	return l; 
}


void GObject::DrawBlank(HDC& hdc) {
//申请资源,并交给只能指针处理
	HBRUSH hbr = ::CreateSolidBrush(RGB(255, 255, 255));
	std::shared_ptr<HBRUSH>phbr(&hbr, [](auto hbr) {
		DeleteObject(*hbr);
		});

	RECT rect;
	rect.top = m_nY - RD;
	rect.left = m_nX - RD;
	rect.right = m_nX + RD;
	rect.bottom = m_nY + RD;
	FillRect(hdc, &rect, *phbr);
}

//设置中心位置
void GObject::SetPosition(int Row, int Array) {
	m_nRow = Row;
	m_nArray = Array;
	this->m_ptCenter.y = m_nRow * pStage->LD + pStage->LD / 2;
	this->m_ptCenter.x = m_nArray * pStage->LD + pStage->LD / 2;
}

//碰撞检测
bool GObject::Collision() {
	bool b = false;

	//更新行和列的数据，若是大嘴，则会执行pacman重写achiveCtrl()函数消除豆子
	AchiveCtrl();
	//判断指令的有效性
	if (m_nArray <0 || m_nRow <0 || m_nArray>MAPLENTH - 1 || m_nRow>MAPLENTH - 1) {
		b = true;
	}
	else if (Achive()) {
		switch (m_cmd)
		{
		case LEFT:
			if (m_nArray > 0 &&
				!pStage->mapData[m_nRow][m_nArray - 1]) {
				b = true;
			}
			break;
			//以下方向的判断原理相同
		case RIGHT:
			if (m_nArray < MAPLENTH - 1 &&
				!pStage->mapData[m_nRow][m_nArray + 1]) {
				b = true;
			}
			break;
		case UP:
			if (m_nRow > 0 && !pStage->mapData[m_nRow - 1][m_nArray]) {
				b = true;
			}
			break;
		case DOWN:
			if (m_nRow < MAPLENTH - 1 && !pStage->mapData[m_nRow + 1][m_nArray]) {
				b = true;
			}
			break;
		}
		if (!b) {
			m_dir = m_cmd; //没撞墙 指令成功
		}
	}

	//依照真实的方向位移
	m_nX = m_ptCenter.x;
	m_nY = m_ptCenter.y;
	int MAX = pStage->LD * MAPLENTH + pStage->LD / 2;
	int MIN = pStage->LD / 2;
	switch (m_dir)
	{
	case UP:
		if (m_nRow > 0 && !pStage->mapData[m_nRow - 1][m_nArray]) {
			b = true;
			break;
		}
		m_ptCenter.y -= m_nSpeed;
		if (m_ptCenter.y < MIN) {
			m_ptCenter.y = MAX;
		}
		break;


	case DOWN:
		if (m_nRow < MAPLENTH - 1 && !pStage->mapData[m_nRow + 1][m_nArray]) {
			b = true;
			break;
		}
		m_ptCenter.y += m_nSpeed;
		if (m_ptCenter.y > MAX) {
			m_ptCenter.y = MIN;
		}
		
	case LEFT:
		if (m_nArray > 0 && !pStage->mapData[m_nRow][m_nArray - 1]) {
			b = true;
			break;
		}
		m_ptCenter.x -= m_nSpeed;
		if (m_ptCenter.x < MIN) {
			m_ptCenter.x = MAX;
		}
		break;

	case RIGHT:
		if (m_nArray < MAPLENTH - 1 && !pStage->mapData[m_nRow][m_nArray + 1]) {
			b = true;
			break;
		}
		m_ptCenter.x += m_nSpeed;
		if (m_ptCenter.x >MAX) {
			m_ptCenter.x = MIN;
		}
		break;
	}
	return b;
}


//玩家对象的实现
//pacman成员定义
void PacMan::AchiveCtrl() {
	GObject::AchiveCtrl();
	if (Achive()) {
		if (m_nRow >= 0 && m_nRow < MAPLENTH && m_nArray >= 0 && m_nArray < MAPLENTH) {
		//阻止数组越界
			if (pStage->peaMapData[m_nRow][m_nArray]) {
				pStage->peaMapData[m_nRow][m_nArray] = false;
			}
		}
	}
}
//进行碰撞检测
void PacMan::action() {
	Collision(); 
}
//设置移动方向
void PacMan::SetTwCommand(TWARDS command) {
	m_cmd = command; 
}
//判断游戏是否结束
bool PacMan::IsOver()
{
	return m_dir == OVER;
}
bool PacMan::IsWin() {
	for (int i = 0; i <= MAPLENTH; i++) {
		for (int j = 0; j <= MAPLENTH; j++) {
			if (pStage->peaMapData[i][j] == true) {  //是豆子
				return false; //存在任意一个豆子，没取得胜利
			}
		}
	}
	return true;  //没有豆子 胜利
}

//返回对象的中心位置
POINT PacMan::GetPos() {
	return m_ptCenter;
}
//设置游戏结束
void PacMan::SetOver() {
	m_dir = OVER; 
}

void PacMan::Draw(HDC& memDC) {
	if (m_dir == OVER) {
	//游戏结束，什么也不干
	}
	//第4帧动画与第二帧动画，张嘴形状
	else if (m_nFrame % 2 == 0) {
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
		int offsetX = DISTANCE / 2 + D_OFFSET;
		int offsetY = DISTANCE / 2 + D_OFFSET;
		switch (m_dir)
		{
		case UP:
			x1 = m_ptCenter.x - offsetX;
			x2 = m_ptCenter.x + offsetX;
			y2 = y1 = m_ptCenter.y - offsetY;
			break;
		case DOWN:
			x1 = m_ptCenter.x + offsetX;
			x2 = m_ptCenter.x - offsetX;
			y2 = y1 = m_ptCenter.y + offsetY;
			break;
		case LEFT:
			x2=x1 = m_ptCenter.x - offsetX;
			y1 = m_ptCenter.y + offsetY;
			y2 =  m_ptCenter.y - offsetY;
			break;
		case RIGHT:
			x2 = x1 = m_ptCenter.x + offsetX;
			y1 = m_ptCenter.y - offsetY;
			y2 = m_ptCenter.y + offsetY;
			break;
		}

		//画出弧线部分
		Arc(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE, x1, y1, x2, y2);

		//画出直线部分
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, m_ptCenter.x, m_ptCenter.y);
		LineTo(memDC, x2, y2);
	}
	//第三帧动画：画出整个圆形
	else if (m_nFrame % 3 == 0) {
		Ellipse(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE);
	}
	//嘴完全张开的形状
	else {
		int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
		switch (m_dir)
		{
		case UP:
			x1 = m_ptCenter.x - DISTANCE;
			x2 = m_ptCenter.x + DISTANCE;
			y2 = y1 = m_ptCenter.y;
			break;
		case DOWN:
			x1 = m_ptCenter.x + DISTANCE;
			x2 = m_ptCenter.x - DISTANCE;
			y2 = y1 = m_ptCenter.y;
			break;

		case LEFT:
			x2 =x1 = m_ptCenter.x;
			y1 = m_ptCenter.y + DISTANCE;
			y2 =m_ptCenter.y - DISTANCE;
			break;

			
		case RIGHT:
			x2 = x1 = m_ptCenter.x;
			y1 = m_ptCenter.y - DISTANCE;
			y2 = m_ptCenter.y + DISTANCE;
			break;
		}
		//画出弧线部分
		Arc(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE, x1, y1, x2, y2);

		//画出直线部分
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, m_ptCenter.x, m_ptCenter.y);
		LineTo(memDC, x2, y2);

	}
	m_nFrame++; //绘制下一帧
}
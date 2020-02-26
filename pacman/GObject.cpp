#include "GObject.h"
#include "framework.h"

//GOject��Ա����
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
//�����߼�����������
void GObject::AchiveCtrl()
{
	if (Achive()) { //����ﵽ�߼�����
		m_nArray = PtTransform(m_ptCenter.x); ////������
		m_nRow = PtTransform(m_ptCenter.y); //������
	}
}

//�ж������Ƿ񵽴��߼�����λ��
bool GObject::Achive() {
	int n = (m_ptCenter.x - pStage->LD / 2) % pStage->LD;
	int k = (m_ptCenter.y - pStage->LD / 2) % pStage->LD; //����Y���������
	bool l = (n == 0 && k == 0); //�����������Ϊ0��˵����������λ��
	return l; 
}


void GObject::DrawBlank(HDC& hdc) {
//������Դ,������ֻ��ָ�봦��
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

//��������λ��
void GObject::SetPosition(int Row, int Array) {
	m_nRow = Row;
	m_nArray = Array;
	this->m_ptCenter.y = m_nRow * pStage->LD + pStage->LD / 2;
	this->m_ptCenter.x = m_nArray * pStage->LD + pStage->LD / 2;
}

//��ײ���
bool GObject::Collision() {
	bool b = false;

	//�����к��е����ݣ����Ǵ��죬���ִ��pacman��дachiveCtrl()������������
	AchiveCtrl();
	//�ж�ָ�����Ч��
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
			//���·�����ж�ԭ����ͬ
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
			m_dir = m_cmd; //ûײǽ ָ��ɹ�
		}
	}

	//������ʵ�ķ���λ��
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


//��Ҷ����ʵ��
//pacman��Ա����
void PacMan::AchiveCtrl() {
	GObject::AchiveCtrl();
	if (Achive()) {
		if (m_nRow >= 0 && m_nRow < MAPLENTH && m_nArray >= 0 && m_nArray < MAPLENTH) {
		//��ֹ����Խ��
			if (pStage->peaMapData[m_nRow][m_nArray]) {
				pStage->peaMapData[m_nRow][m_nArray] = false;
			}
		}
	}
}
//������ײ���
void PacMan::action() {
	Collision(); 
}
//�����ƶ�����
void PacMan::SetTwCommand(TWARDS command) {
	m_cmd = command; 
}
//�ж���Ϸ�Ƿ����
bool PacMan::IsOver()
{
	return m_dir == OVER;
}
bool PacMan::IsWin() {
	for (int i = 0; i <= MAPLENTH; i++) {
		for (int j = 0; j <= MAPLENTH; j++) {
			if (pStage->peaMapData[i][j] == true) {  //�Ƕ���
				return false; //��������һ�����ӣ�ûȡ��ʤ��
			}
		}
	}
	return true;  //û�ж��� ʤ��
}

//���ض��������λ��
POINT PacMan::GetPos() {
	return m_ptCenter;
}
//������Ϸ����
void PacMan::SetOver() {
	m_dir = OVER; 
}

void PacMan::Draw(HDC& memDC) {
	if (m_dir == OVER) {
	//��Ϸ������ʲôҲ����
	}
	//��4֡������ڶ�֡������������״
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

		//�������߲���
		Arc(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE, x1, y1, x2, y2);

		//����ֱ�߲���
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, m_ptCenter.x, m_ptCenter.y);
		LineTo(memDC, x2, y2);
	}
	//����֡��������������Բ��
	else if (m_nFrame % 3 == 0) {
		Ellipse(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE);
	}
	//����ȫ�ſ�����״
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
		//�������߲���
		Arc(memDC, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE, x1, y1, x2, y2);

		//����ֱ�߲���
		MoveToEx(memDC, x1, y1, NULL);
		LineTo(memDC, m_ptCenter.x, m_ptCenter.y);
		LineTo(memDC, x2, y2);

	}
	m_nFrame++; //������һ֡
}

//�о������Ա�������
//enermy��Ա���壺
std::shared_ptr<PacMan> Enermy::player = nullptr;

//ץס ��Ϸ����
void Enermy::Catch() {
	int DX = m_ptCenter.x - player->GetPos.x;
	int DY = m_ptCenter.y - player->GetPos().y;
	if ((-RD < DX && DX < RD) && (-RD < DY && DY < RD)) {
		player->SetOver();
	}
}

void Enermy::Draw(HDC& hdc) {
	HPEN pen = ::CreatePen(0, 0, color);
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);

	//��һ����Բ�ε�ͷ
	Arc(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y - DISTANCE,
		m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE,
		m_ptCenter.x + DISTANCE, m_ptCenter.y,
		m_ptCenter.x - DISTANCE, m_ptCenter.y);

	int const LEGLENTH = (DISTANCE) / (LEGCOUNTS);

	//����֡������������͡��Ȳ���
	if (m_nFrame % 2 == 0) {
	//���ε�����
		MoveToEx(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y, NULL);
		LineTo(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y + DISTANCE - LEGLENTH);
		MoveToEx(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y, NULL);
		LineTo(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE - LEGLENTH);

		for (int i = 0; i < LEGCOUNTS; i++) {
			Arc(hdc,
				m_ptCenter.x - DISTANCE + i * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE - 2 * LEGLENTH,
				m_ptCenter.x - DISTANCE + (i + 1) * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE,
				m_ptCenter.x - DISTANCE + i * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE - LEGLENTH,
				m_ptCenter.x - DISTANCE + (i + 1) * 2 * LEGLENTH,
				m_ptCenter.y + DISTANCE - LEGLENTH
			);
		}
	}
	//��������
	else {
		MoveToEx(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y, NULL); 
		LineTo(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y + DISTANCE);
		MoveToEx(hdc, m_ptCenter.x+ DISTANCE, m_ptCenter.y, NULL);
		LineTo(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE);

		MoveToEx(hdc, m_ptCenter.x - DISTANCE, m_ptCenter.y+DISTANCE, NULL);
		LineTo(hdc, m_ptCenter.x - DISTANCE+LEGLENTH, m_ptCenter.y + DISTANCE- LEGLENTH);

		//�������һ��ơ��Ȳ���
		for (int i = 0; i < LEGCOUNTS - 1; i++) {
			Arc(hdc,
				m_ptCenter.x - DISTANCE + (1 + i * 2) * LEGLENTH,
				m_ptCenter.y + DISTANCE - 2 * LEGLENTH,
				m_ptCenter.x - DISTANCE + (3 + i * 2) * LEGLENTH,
				m_ptCenter.y + DISTANCE,
				m_ptCenter.x - DISTANCE + (1 + i * 2) * LEGLENTH,
				m_ptCenter.y + DISTANCE - LEGLENTH,
				m_ptCenter.x - DISTANCE + (3 + i * 2) * LEGLENTH,
				m_ptCenter.y + DISTANCE - LEGLENTH
			);
		}
		MoveToEx(hdc, m_ptCenter.x + DISTANCE, m_ptCenter.y + DISTANCE, NULL);
		LineTo(hdc, m_ptCenter.x + DISTANCE - LEGLENTH, m_ptCenter.y + DISTANCE - LEGLENTH);
	}

	//���ݷ�������۾�
	int R = DISTANCE / 5;
	switch (m_dir) //�ֱ����ۺ�����
	{
	case UP:
		Ellipse(hdc, m_ptCenter.x - 2 * R, m_ptCenter.y - 2 * R, m_ptCenter.x, m_ptCenter.y);
		Ellipse(hdc, m_ptCenter.x , m_ptCenter.y - 2 * R, m_ptCenter.x+2*R, m_ptCenter.y);
		break;

	case DOWN:
		Ellipse(hdc, m_ptCenter.x - 2 * R, m_ptCenter.y , m_ptCenter.x, m_ptCenter.y + 2 * R);
		Ellipse(hdc, m_ptCenter.x, m_ptCenter.y, m_ptCenter.x + 2 * R, m_ptCenter.y + 2 * R);
		break;

	case LEFT:
		Ellipse(hdc, m_ptCenter.x - 3 * R, m_ptCenter.y-R, m_ptCenter.x-R, m_ptCenter.y+ R);
		Ellipse(hdc, m_ptCenter.x -  R, m_ptCenter.y - R, m_ptCenter.x + R, m_ptCenter.y + R);

		break;
	case RIGHT:
		Ellipse(hdc, m_ptCenter.x - R, m_ptCenter.y - R, m_ptCenter.x + R, m_ptCenter.y + R);
		Ellipse(hdc, m_ptCenter.x +R, m_ptCenter.y - R, m_ptCenter.x + 3 * R, m_ptCenter.y + R);
		break;
	}
	m_nFrame++; //׼��������һ֡��
	SelectObject(hdc, oldPen); //��ԭ����
	DeleteObject(pen);//ɾ�����ʶ���
	return;
}

void Enermy::action() {
	bool b = Collision(); //�ж��Ƿ�����ײ
	MakeDecision(b); //�趨����
	Catch (); //��ʼץ��
}

//redone��Ա
void RedOne::Draw(HDC& hdc) {
	Enermy::Draw(hdc);
}

void RedOne::MakeDecision(bool b) {
//srand(time(0))
	int i = rand();
	if (b) { //ײ��ǽ�ڣ��ı䷽��
		if (i % 4 == 0) {
			m_dir == UP ? m_cmd = LEFT : m_cmd = UP; //������, �����
		}
		else if (i % 3 == 0) {
			m_dir == DOWN ? m_cmd = UP : m_cmd = RIGHT; //�����ң����Ϲ�
		}
		else {
			m_dir == LEFT ? m_cmd = DOWN : m_cmd = LEFT; //������,���¹�
		}
		return;
	}
	//�������е����˵��û��ײǽ����������
	if (i % 4 == 0) {
		m_cmd != UP ? m_dir == DOWN : m_cmd == UP;
	}
	else if (i % 3 == 0) {
		m_dir != DOWN ? m_cmd = UP : m_cmd = DOWN;
	}
	else if (i % 2 == 0) {
		m_dir != RIGHT ? m_cmd = LEFT : m_cmd = RIGHT;
	}
	else {
		m_dir != LEFT ? m_cmd = RIGHT : m_cmd = LEFT;
	}
}
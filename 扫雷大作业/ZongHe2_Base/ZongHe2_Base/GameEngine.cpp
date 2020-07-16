/****************************************
GameEngine.cpp
Author: Root
Date: 2019/12/09
Description:
�����˳�����Ϸģ�飬��GameEngine�����ռ��ֹͬ����Ⱦ��������Ҫ��Ҫ�޸ĵ��ļ�
*****************************************/
#include "GameEngine.h"

namespace GameEngine {
	// �������������ռ�
	using namespace Graphic;
	using namespace Controller;

	size_t mapWidth = 0, mapHeight = 0;				 // ��ͼ��Ⱥ͸߶�
	UCHAR mapCanvas[50 * 25];						 // ��ͼ����ָ�룬�����������벻Ҫֱ�ӳ��Բ������ָ��
	COORD pos = { 0 }, posOld = { 0 };				 // ����λ��
	COORD posChoice = { 0 }, posChoiceOld = { 0 };	 // ѡ�е�ͼ�����λ��
	char key = '\0';								 // ���̵İ���
	bool gameFlag = false;							 // ��Ϸ����״̬
	size_t frame = 0;								 // ����Ⱦ֡��
	clock_t tic = clock();							 // ��Ϸ��ʼʱ��
	int operation = 0;								 // ִ�еĲ���
	bool isFirst = true;							 // �Ƿ�Ϊ��һ��
	//int mapArray[50][50];							 // ����ֱ�Ӳ���mapCanvas����ʹ��mapArray����

	// ��������¼ӵı�������������·���Ԥ�ñ���������
	int boomTotal = 0;								 // �����׵�����
	int safeZone = 0;								 // ���岻����Ϊ�׵İ�ȫ��������λ��
	UCHAR mapVisible[50 * 25];					 // ���嵱ǰ�ɼ��ĵ�ͼ��Ϣ
	int remainBlock = 0;							 // ����δ�㿪��ש����
	int flagNum = 0;								 // �����Ѿ��������Ŀ
	int alreadySlice = 1;							 // �������ߵĲ���
	double timeStart, timeNow, timeEnd = 1000;		 // ���忪ʼʱ��ʱ��


	/****************************************
	Function:  renderMap()
	Parameter: None(void)
	Return:    None(void)
	Description:
	���Ƶ�ǰ��ͼ
	*****************************************/
	void renderMap() {

		// ��0,0�����õ�ǰѡ��λ�õ��ַ�����ע���β�пո����ף�����˼��ΪʲôҪ����ô��ո�
		MovePos(0, 1);
		char strPos[32] = "";
		sprintf_s(strPos, "��ǰѡ��(%u, %u)         ", posChoice.X, posChoice.Y);
		PutString(strPos);
		Update();

		// ��0,20�������ַ�������ʾʱ��
		MovePos(20, 1);
		char strTime[32] = "";
		if (isFirst) {
			sprintf_s(strTime, "����ʱ�䣺%u         ", 0);
		}
		else {
			sprintf_s(strTime, "����ʱ�䣺%u         ", (int)(timeEnd - timeStart) / 1000);
		}
		PutString(strTime);
		Update();

		// ��1,0�������ַ�������ʾʣ������
		MovePos(0, 2);
		char strBoom[32] = "";
		sprintf_s(strBoom, "ʣ��������%d         ", boomTotal - flagNum);
		PutString(strBoom);
		Update();

		// ��1,20�������ַ�������ʾ���߲���
		MovePos(20, 2);
		char strSlice[32] = "";
		sprintf_s(strSlice, "���߲�����%u         ", (isFirst ? 0 : alreadySlice));
		PutString(strSlice);
		Update();

		// ���п�ʼ����
		for (size_t i = 0; i < mapHeight + 1; i++) {
			// �ƶ����������� ���������׵��ַ�
			MovePos(0, (SHORT)i * 2 + 3);
			if (i == 0) {
				PutString("�X");
			}
			else if (i == mapHeight) {
				PutString("�^");
			}
			else {
				PutString("�d");
			}

			// �����У����Ʊ߽�
			for (size_t j = 0; j < mapWidth; j++) {
				// �ƶ���ȷ�е����꣬���Ʒ���ı߽�
				MovePos(2 + (SHORT)j * 8, (SHORT)i * 2 + 3);
				if (i == 0) {
					if (j < mapWidth - 1) {
						PutString("�T�T�T�j");
					}
					else {
						PutString("�T�T�T�[");
					}
				}
				else if (i == mapHeight) {
					if (j < mapWidth - 1) {
						PutString("�T�T�T�m");
					}
					else {
						PutString("�T�T�T�a");
					}
				}
				else {
					if (j < mapWidth - 1) {
						PutString("�T�T�T�p");
					}
					else {
						PutString("�T�T�T�g");
					}
				}
			}
			// ���Ƶ��׵�ͼ
			if (i > 0 && i < mapHeight + 1) {
				// �ƶ������ף����Ʊ߽��ַ�
				MovePos(0, (SHORT)i * 2 + 2);
				PutString("�U");
				// ������ ���Ƶ���
				for (size_t j = 0; j < mapWidth; j++) {
					MovePos(2 + (SHORT)j * 5, (SHORT)i * 2 + 2);    // �ƶ���ȷ������
					const size_t mapIndex = (i - 1) * mapWidth + j; // ȷ����ͼ������±�
					char numMap[8] = "   ";							// ȷ�������ַ���
					numMap[1] = '0' + mapCanvas[mapIndex];			// ��mapVisible[mapIndex]Ϊ1��8ʱ������ת�����ַ���
					switch (mapVisible[mapIndex]) {
					case 0:
						switch (mapCanvas[mapIndex]) {
						case 0:
							// 0��ʱ����ÿհ�
							PutString("   ");
							break;
						case 1:
							// ��1��ʼ�������� ����ɫ
							PutStringWithColor(numMap, 30, 144, 255, 0, 0, 0);
							break;
						case 2:
							PutStringWithColor(numMap, 0, 255, 127, 0, 0, 0);
							break;
						case 3:
							PutStringWithColor(numMap, 255, 48, 48, 0, 0, 0);
							break;
						case 4:
							PutStringWithColor(numMap, 72, 61, 139, 0, 0, 0);
							break;
						case 5:
							PutStringWithColor(numMap, 255, 105, 180, 0, 0, 0);
							break;
						case 6:
							PutStringWithColor(numMap, 148, 0, 211, 0, 0, 0);
							break;
						case 7:
							PutStringWithColor(numMap, 139, 0, 0, 0, 0, 0);
							break;
						case 8:
							PutStringWithColor(numMap, 139, 34, 82, 0, 0, 0);
							break;

						case 11:
							// 11Ϊ����
							PutStringWithColor(" ��", 255, 215, 0, 0, 0, 0);
							break;
						}
						break;
					case 9:
						// 9Ϊש��
						PutString(" �~");
						break;
					case 10:
						// 10Ϊ����
						PutStringWithColor(" ��", 255, 255, 255, 0, 0, 0);
						break;
					case 12:
						//12Ϊ�ݶ�
						PutStringWithColor(" ��", 255, 255, 255, 0, 0, 0);
						break;
					}

					MovePos(5 + (SHORT)j * 5, (SHORT)i * 2 + 2);
					PutString("�U");
				}
			}
		}
		Update(); // ����ͼ���µ���Ļ
	}

	/****************************************
	Function:  updateMap()
	Parameter: None(void)
	Return:    None(void)
	Description:
	����ά��������ݸ��Ƶ�һά������
	*****************************************/
	/*void updateMap() {
		memcpy_s(mapCanvas, mapWidth * mapHeight, mapArray, mapWidth * mapHeight);
	}*/

	/****************************************
	Function:  InitGame(int getWidth,int getHeight, int getBoom)
	Parameter: getWidth(int),getHeight(int),getBoom(int)
	Return:    None(void)
	Description:
	��ʼ����Ϸ
	*****************************************/
	void InitGame(int getWidth,int getHeight, int getBoom) {
		ClearScreen();								 // ����
		mapWidth = getWidth; 
		mapHeight = getHeight;						// ���ÿ��
		boomTotal = getBoom;

		for (int i = 0; i < mapWidth * mapHeight; i++) {		// ��ʼ����������
			mapCanvas[i] = 0;
			mapVisible[i] = 9;
		}
		//updateMap();	// ���µ�ͼ
		remainBlock = mapWidth * mapHeight;
		renderMap();	// ���Ƶ�ͼ
	}

	/****************************************
	Function:  renderChoiceBackground(COORD choice)
	Parameter: COORD choice
	Return:    None(void)
	Description:
	����ѡ�еĵؿ�ı���
	*****************************************/
	void renderChoiceBackground(COORD choice) {
		const SHORT x = choice.X, y = choice.Y;
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);        // ���屳����ɫ��ǰ����ɫֵ��ͨ������Ⱦ��֡���������㣨�˴�Ҳ���Ըĳ�ʹ��ʱ�䣩
		const size_t mapIndex = (size_t)y * mapWidth + (size_t)x; // ȷ����ͼ�±�

		MovePos(2 + x * 4, y * 2 + 3);

		// ��������ͬrenderMap�л��Ƶ��ײ�����ͬ������������
		char numMap[8] = "  ";
		numMap[0] = '0' + mapCanvas[mapIndex];
		switch (mapVisible[mapIndex]) {
		case 0:
			switch (mapCanvas[mapIndex]) {
			case 0:
				PutStringWithColor("  ", 255, 255, 255, alpha, alpha, alpha);
				break;
			case 1:
				PutStringWithColor(numMap, 30, 144, 255, alpha, alpha, alpha);
				break;
			case 2:
				PutStringWithColor(numMap, 0, 255, 127, alpha, alpha, alpha);
				break;
			case 3:
				PutStringWithColor(numMap, 255, 48, 48, alpha, alpha, alpha);
				break;
			case 4:
				PutStringWithColor(numMap, 72, 61, 139, alpha, alpha, alpha);
				break;
			case 5:
				PutStringWithColor(numMap, 255, 105, 180, alpha, alpha, alpha);
				break;
			case 6:
				PutStringWithColor(numMap, 148, 0, 211, alpha, alpha, alpha);
				break;
			case 7:
				PutStringWithColor(numMap, 139, 0, 0, alpha, alpha, alpha);
				break;
			case 8:
				PutStringWithColor(numMap, 139, 34, 82, alpha, alpha, alpha);
				break;
			case 11:
				PutStringWithColor("��", 255, 215, 0, alpha, alpha, alpha);
				break;
			}
			break;
		case 9:
			PutStringWithColor("�~", 255, 255, 255, alpha, alpha, alpha);
			break;
		case 10:
			PutStringWithColor("��", 255, 255, 255, alpha, alpha, alpha);
			break;
		case 12:
			PutStringWithColor("��", 255, 255, 255, alpha, alpha, alpha);
			break;
		}
	}

	/****************************************
	Function:  clearChoiceBackground(COORD choice)
	Parameter: COORD choice
	Return:    None(void)
	Description:
	���choice��ָʾλ�õı���������ͬ�ϣ�����������
	*****************************************/
	void clearChoiceBackground(COORD choice) {
		const SHORT x = choice.X, y = choice.Y;
		const size_t mapIndex = (size_t)y * mapWidth + (size_t)x;

		MovePos(2 + x * 4, y * 2 + 3);

		char numMap[8] = "  ";
		numMap[0] = '0' + mapCanvas[mapIndex];
		switch (mapVisible[mapIndex]) {
		case 0:
			switch (mapCanvas[mapIndex]) {
			case 0:
				PutStringWithColor("  ", 255, 255, 255, 0, 0, 0);
				break;
			case 1:
				PutStringWithColor(numMap, 30, 144, 255, 0, 0, 0);
				break;
			case 2:
				PutStringWithColor(numMap, 0, 255, 127, 0, 0, 0);
				break;
			case 3:
				PutStringWithColor(numMap, 255, 48, 48, 0, 0, 0);
				break;
			case 4:
				PutStringWithColor(numMap, 72, 61, 139, 0, 0, 0);
				break;
			case 5:
				PutStringWithColor(numMap, 255, 105, 180, 0, 0, 0);
				break;
			case 6:
				PutStringWithColor(numMap, 148, 0, 211, 0, 0, 0);
				break;
			case 7:
				PutStringWithColor(numMap, 139, 0, 0, 0, 0, 0);
				break;
			case 8:
				PutStringWithColor(numMap, 139, 34, 82, 0, 0, 0);
				break;
			case 11:
				PutStringWithColor("��", 255, 215, 0, 0, 0, 0);
				break;
			}
			break;
		case 9:
			PutStringWithColor("�~", 255, 255, 255, 0, 0, 0);
			break;
		case 10:
			PutStringWithColor("��", 255, 255, 255, 0, 0, 0);
			break;
		case 12:
			PutStringWithColor("��", 255, 255, 255, 0, 0, 0);
			break;
		}
	}

	/****************************************
	Function:  checkChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	�������Ͳ�������������ر���
	*****************************************/
	void checkChoice() {
		FlushInput();							// ˢ�����뻺����
		pos = GetCursorPos();					// ��ȡ�������
		COORD hitLeftPos = GetCursorHitPos();	// ��ȡ�����������
		COORD hitRightPos = GetCursorHitPos(2);	// ��ȡ�Ҽ���������
		key = GetKeyHit();						// ��ȡ��������
		operation = 0;							// ��ǰ���� (0�޲�����1Ϊ�ڿ��ؿ飬2Ϊ��ֵ���ģ������Լ�ָ��)

		// ����������
		switch (key) {
		case VK_ESCAPE:
			// ESC��������Ϸ����״̬�÷����˳���Ϸ
			gameFlag = false;
			break;

		// �������Ҽ��ƶ�ѡ�е�����
		case VK_UP:
			if (posChoice.Y > 0)posChoice.Y--;
			break;
		case VK_RIGHT:
			if (posChoice.X < (SHORT)mapWidth - 1)posChoice.X++;
			break;
		case VK_DOWN:
			if (posChoice.Y < (SHORT)mapHeight - 1)posChoice.Y++;
			break;
		case VK_LEFT:
			if (posChoice.X > 0)posChoice.X--;
			break;

		// �س��Ϳո�ֱ��Ӧ����1��2
		case VK_RETURN:
			operation = 1;
			break;
		case VK_SPACE:
			operation = 2;
			break;
		}

		// ��������̨�������������ͼ����
		const size_t mouseY = (size_t)((size_t)pos.Y + 1) / 2 - 2;
		const size_t mouseX = (size_t)((size_t)pos.X - 1) / 4;
		if (mouseY < mapHeight && mouseX < mapWidth) {
			// ��û�г��߽磬����ѡ�������
			posChoice.X = (SHORT)mouseX;
			posChoice.Y = (SHORT)mouseY;
		}

		// ������Ҽ��ֱ��Ӧ����1��2
		if (hitLeftPos.X == pos.X && hitLeftPos.Y == pos.Y) {
			operation = 1;
		}
		else if (hitRightPos.X == pos.X && hitRightPos.Y == pos.Y) {
			operation = 2;
		}
	}

	/****************************************
	Function:  isInArray()
	Parameter: ins(int),arr(int[9])
	Return:    1/0 (bool)
	Description:
	�ж�ins�Ƿ��arr�е�ĳ��Ԫ����ȣ���ȵĻ�����true
	*****************************************/
	bool isInArray(int ins, int arr[]) {
		for (int i = 0; i < 9; i++) {
			if (ins == arr[i]) {
				return true;
			}
		}
		return false;
	}

	/****************************************
	Function:  initMap()
	Parameter: None(void)
	Return:    None(void)
	Description:
	��ʼ����������ͨ��posChoice�����жϲ�����������mapArray���飬�벻Ҫ������������Ԥ�úõı���
	*****************************************/
	void initMap() {
		int boomCounter = 0;									//�����Ѿ������׵ļ�����

		int tempBomb = 0;										//�������Ϊ�׵Ĺ�����
		UCHAR* pin = mapCanvas;									//�������׹���ָ��

		int safePlace[9] = { -1,-1,-1,-1,-1,-1,-1,-1,-1 };		//���尲ȫ�����巶Χ
		if (safeZone >= mapWidth) {
			for (int i = 0; i < 3; i++) {
				safePlace[i] = safeZone - mapWidth - 1 + i;
			}
		}
		for (int i = 3; i < 6; i++) {
			safePlace[i] = safeZone - 4 + i;
		}
		if (safeZone < (mapHeight - 1) * mapWidth) {
			for (int i = 6; i < 9; i++) {
				safePlace[i] = safeZone + mapWidth - 7 + i;
			}
		}
		if (safeZone % mapWidth == 0) {
			for (int i = 0; i < 3; i++) {
				safePlace[3 * i] = -1;
			}
		}
		if (safeZone % mapWidth == mapWidth - 1) {
			for (int i = 0; i < 3; i++) {
				safePlace[3 * i + 2] = -1;
			}
		}

		int safeSize = 9;										//���尲ȫ���Ĵ�С
		if (posChoice.X == 0 || posChoice.X == mapWidth - 1) {
			safeSize -= 3;
		}
		if (posChoice.Y == 0 || posChoice.Y == mapHeight - 1) {
			safeSize = (safeSize == 9 ? 6 : 4);
		}

		srand((unsigned)time(NULL));							//����ʱ����Ϊ���������

		for (;;) {
			pin = mapCanvas;
			tempBomb = rand() % (mapWidth * mapHeight - safeSize - boomCounter);
			for (int i = 0;;) {
				for (;;) {
					if (*pin >= 9) {				//�ж�û������
						pin++;
					}
					else if (isInArray((int)(pin-mapCanvas),safePlace)) {
						pin++;						//��ֹ�������ڰ�ȫ��
					}
					else {
						break;
					}
				}
				if (i == tempBomb) {							//����������׵��������Χλ����и�ֵ
					*pin = 11;
					if ((pin - mapCanvas) % mapWidth == 0) {
						if ((pin - mapCanvas) >= 0 && (pin - mapCanvas) < mapWidth) {
							*(pin + 1) = (*(pin + 1) == 11 ? 11 : (*(pin + 1) + 1));
							*(pin + mapWidth) = (*(pin + mapWidth) == 11 ? 11 : (*(pin + mapWidth) + 1));
							*(pin + mapWidth + 1) = (*(pin + mapWidth + 1) == 11 ? 11 : (*(pin + mapWidth + 1) + 1));
						}
						else if ((pin - mapCanvas) >= mapWidth * (mapHeight - 1) && (pin - mapCanvas) < mapHeight * mapWidth) {
							*(pin + 1) = (*(pin + 1) == 11 ? 11 : (*(pin + 1) + 1));
							*(pin - mapWidth) = (*(pin - mapWidth) == 11 ? 11 : (*(pin - mapWidth) + 1));
							*(pin - mapWidth + 1) = (*(pin - mapWidth + 1) == 11 ? 11 : (*(pin - mapWidth + 1) + 1));
						}
						else {
							*(pin - mapWidth) = (*(pin - mapWidth) == 11 ? 11 : (*(pin - mapWidth) + 1));
							*(pin - mapWidth + 1) = (*(pin - mapWidth + 1) == 11 ? 11 : (*(pin - mapWidth + 1) + 1));
							*(pin + 1) = (*(pin + 1) == 11 ? 11 : (*(pin + 1) + 1));
							*(pin + mapWidth) = (*(pin + mapWidth) == 11 ? 11 : (*(pin + mapWidth) + 1));
							*(pin + mapWidth + 1) = (*(pin + mapWidth + 1) == 11 ? 11 : (*(pin + mapWidth + 1) + 1));
						}
					}
					else if ((pin - mapCanvas) % mapWidth == mapWidth - 1) {
						if ((pin - mapCanvas) >= 0 && (pin - mapCanvas) < mapWidth) {
							*(pin - 1) = (*(pin - 1) == 11 ? 11 : (*(pin - 1) + 1));
							*(pin + mapWidth) = (*(pin + mapWidth) == 11 ? 11 : (*(pin + mapWidth) + 1));
							*(pin + mapWidth - 1) = (*(pin + mapWidth - 1) == 11 ? 11 : (*(pin + mapWidth - 1) + 1));
						}
						else if ((pin - mapCanvas) >= mapWidth * (mapHeight - 1) && (pin - mapCanvas) < mapHeight * mapWidth) {
							*(pin - 1) = (*(pin - 1) == 11 ? 11 : (*(pin - 1) + 1));
							*(pin - mapWidth) = (*(pin - mapWidth) == 11 ? 11 : (*(pin - mapWidth) + 1));
							*(pin - mapWidth - 1) = (*(pin - mapWidth - 1) == 11 ? 11 : (*(pin - mapWidth - 1) + 1));
						}
						else {
							*(pin - mapWidth) = (*(pin - mapWidth) == 11 ? 11 : (*(pin - mapWidth) + 1));
							*(pin - mapWidth - 1) = (*(pin - mapWidth - 1) == 11 ? 11 : (*(pin - mapWidth - 1) + 1));
							*(pin - 1) = (*(pin - 1) == 11 ? 11 : (*(pin - 1) + 1));
							*(pin + mapWidth) = (*(pin + mapWidth) == 11 ? 11 : (*(pin + mapWidth) + 1));
							*(pin + mapWidth - 1) = (*(pin + mapWidth - 1) == 11 ? 11 : (*(pin + mapWidth - 1) + 1));
						}
					}
					else if ((pin - mapCanvas) >= 0 && (pin - mapCanvas) < mapWidth) {
						*(pin - 1) = (*(pin - 1) == 11 ? 11 : (*(pin - 1) + 1));
						*(pin + 1) = (*(pin + 1) == 11 ? 11 : (*(pin + 1) + 1));
						*(pin + mapWidth) = (*(pin + mapWidth) == 11 ? 11 : (*(pin + mapWidth) + 1));
						*(pin + mapWidth - 1) = (*(pin + mapWidth - 1) == 11 ? 11 : (*(pin + mapWidth - 1) + 1));
						*(pin + mapWidth + 1) = (*(pin + mapWidth + 1) == 11 ? 11 : (*(pin + mapWidth + 1) + 1));
					}
					else if ((pin - mapCanvas) >= mapWidth * (mapHeight - 1) && (pin - mapCanvas) < mapHeight * mapWidth) {
						*(pin - 1) = (*(pin - 1) == 11 ? 11 : (*(pin - 1) + 1));
						*(pin + 1) = (*(pin + 1) == 11 ? 11 : (*(pin + 1) + 1));
						*(pin - mapWidth) = (*(pin - mapWidth) == 11 ? 11 : (*(pin - mapWidth) + 1));
						*(pin - mapWidth - 1) = (*(pin - mapWidth - 1) == 11 ? 11 : (*(pin - mapWidth - 1) + 1));
						*(pin - mapWidth + 1) = (*(pin - mapWidth + 1) == 11 ? 11 : (*(pin - mapWidth + 1) + 1));
					}
					else {
						*(pin - 1) = (*(pin - 1) == 11 ? 11 : (*(pin - 1) + 1));
						*(pin + 1) = (*(pin + 1) == 11 ? 11 : (*(pin + 1) + 1));
						*(pin - mapWidth) = (*(pin - mapWidth) == 11 ? 11 : (*(pin - mapWidth) + 1));
						*(pin - mapWidth - 1) = (*(pin - mapWidth - 1) == 11 ? 11 : (*(pin - mapWidth - 1) + 1));
						*(pin - mapWidth + 1) = (*(pin - mapWidth + 1) == 11 ? 11 : (*(pin - mapWidth + 1) + 1));
						*(pin + mapWidth) = (*(pin + mapWidth) == 11 ? 11 : (*(pin + mapWidth) + 1));
						*(pin + mapWidth - 1) = (*(pin + mapWidth - 1) == 11 ? 11 : (*(pin + mapWidth - 1) + 1));
						*(pin + mapWidth + 1) = (*(pin + mapWidth + 1) == 11 ? 11 : (*(pin + mapWidth + 1) + 1));
					}
					break;
				}
				else {
					pin++;
					i++;
				}
			}
			boomCounter++;
			if (boomCounter == boomTotal) {
				break;
			}
		}
	}

	/****************************************
	Function:  gameOver()
	Parameter: Nnoe(void)
	Return:    None(void)
	Description:
	��Ϸʧ��
	*****************************************/
	void gameOver() {
		timeEnd = GetTickCount64();
		GameMenu::outChat("���ź�����ʧ���� ");
		GameMenu::WaitChatChoice();
		ClearScreen();
		for (int i = 0; i < mapWidth * mapHeight; i++) {
			if (*(mapCanvas + i) == 11) {
				*(mapVisible + i) = 0;
			}
		}
		renderMap();
		bool quitFlag = true;					// ǿ���˳�
		
		while (quitFlag) {
			FlushInput();							// ˢ�����뻺����
			pos = GetCursorPos();					// ��ȡ�������
			COORD hitLeftPos = GetCursorHitPos();	// ��ȡ�����������
			COORD hitRightPos = GetCursorHitPos(2);	// ��ȡ�Ҽ���������
			key = GetKeyHit();						// ��ȡ��������

			// ����������
			if (key) {
				quitFlag = false;
			}

			// ��������̨�������������ͼ����
			const size_t mouseY = (size_t)((size_t)pos.Y + 1) / 2 - 2;
			const size_t mouseX = (size_t)((size_t)pos.X - 1) / 4;
			if (mouseY < mapHeight && mouseX < mapWidth) {
				// ��û�г��߽磬����ѡ�������
				posChoice.X = (SHORT)mouseX;
				posChoice.Y = (SHORT)mouseY;
			}

			// ������Ҽ��ֱ��Ӧ����1��2
			if (hitLeftPos.X == pos.X && hitLeftPos.Y == pos.Y) {
				quitFlag = false;
			}
			else if (hitRightPos.X == pos.X && hitRightPos.Y == pos.Y) {
				quitFlag = false;
			}
		}
		gameFlag = false;
	}

	/****************************************
	Function:  digBlock(int digPlace)
	Parameter: digPlace(int)
	Return:    None(void)
	Description:
	ʵ�ַ�������
	*****************************************/
	void digBlock(int digPlace) {
		if (isFirst) {
			// ����ǵ�һ���ߣ����ȳ�ʼ����ͼ��ע�ⲻҪ���������һ������
			safeZone = posChoice.Y * mapWidth + posChoice.X;	//������ȫ��
			initMap();
			isFirst = false; // ����һ������Ϊ��
			timeStart = GetTickCount64();
		}
		if (*(mapVisible + digPlace) == 0) {
			return;
		}
		if (*(mapCanvas + digPlace) == 11) {
			gameOver();
		}
		else if (*(mapCanvas + digPlace) >= 1 && *(mapCanvas + digPlace) <= 8) {
			*(mapVisible + digPlace) = 0;
			remainBlock--;
			renderMap();
		}
		else if (*(mapCanvas + digPlace) == 0){		//�����ú��������ķ�ʽ�����ڽ����׷���
			*(mapVisible + digPlace) = 0;
			remainBlock--;
			renderMap();
			if (digPlace >= mapWidth) {
				digBlock(digPlace - mapWidth);
				if (digPlace % mapWidth != mapWidth - 1) {
					digBlock(digPlace - mapWidth + 1);
				}
				if (digPlace % mapWidth != 0) {
					digBlock(digPlace - mapWidth - 1);
				}
			}
			if (digPlace < mapWidth * (mapHeight - 1)) {
				digBlock(digPlace + mapWidth);
				if (digPlace % mapWidth != mapWidth - 1) {
					digBlock(digPlace + mapWidth + 1);
				}
				if (digPlace % mapWidth != 0) {
					digBlock(digPlace + mapWidth - 1);
				}
			}
			if (digPlace % mapWidth != mapWidth - 1) {
				digBlock(digPlace + 1);
			}
			if (digPlace % mapWidth != 0) {
				digBlock(digPlace - 1);
			}
		}
	}

	/****************************************
	Function:  flagBlock(int flagPlace)
	Parameter: flagPlace(int)
	Return:    None(void)
	Description:
	ʵ�ֲ������
	*****************************************/
	void flagBlock(int flagPlace) {
		if (*(mapVisible + flagPlace) == 0) {
			return;
		}
		switch(*(mapVisible + flagPlace)) {
		case 9:
			*(mapVisible + flagPlace) = 10;
			flagNum++;
			break;
		case 10:
			*(mapVisible + flagPlace) = 12;
			flagNum--;
			break;
		case 12:
			*(mapVisible + flagPlace) = 9;
			break;
		}
		renderMap();
	}

	/****************************************
	Function:  judgeWin()
	Parameter: None(void)
	Return:    winFlag(bool)
	Description:
	�����ж��Ƿ���ȫ�����
	*****************************************/
	bool judgeWin() {
		if (remainBlock == boomTotal) {
			timeEnd = GetTickCount64();
			renderMap();
			GameMenu::outChat("��ϲ����Ӫ�ȳɹ� ");
			GameMenu::WaitChatChoice();
			ClearScreen();
			
			for (int i = 0; i < mapWidth * mapHeight; i++) {
				if (*(mapCanvas + i) == 11) {
					*(mapVisible + i) = 0;
				}
			}
			renderMap();
			
			bool quitFlag = true;					// ǿ���˳�

			while (quitFlag) {
				FlushInput();							// ˢ�����뻺����
				pos = GetCursorPos();					// ��ȡ�������
				COORD hitLeftPos = GetCursorHitPos();	// ��ȡ�����������
				COORD hitRightPos = GetCursorHitPos(2);	// ��ȡ�Ҽ���������
				key = GetKeyHit();						// ��ȡ��������
				operation = 0;							// ��ǰ���� (0�޲�����1Ϊ�ڿ��ؿ飬2Ϊ��ֵ���ģ������Լ�ָ��)

				// ����������
				if (key) {
					quitFlag = false;
				}

				// ��������̨�������������ͼ����
				const size_t mouseY = (size_t)((size_t)pos.Y + 1) / 2 - 2;
				const size_t mouseX = (size_t)((size_t)pos.X - 1) / 4;
				if (mouseY < mapHeight && mouseX < mapWidth) {
					// ��û�г��߽磬����ѡ�������
					posChoice.X = (SHORT)mouseX;
					posChoice.Y = (SHORT)mouseY;
				}

				// ������Ҽ��ֱ��Ӧ����1��2
				if (hitLeftPos.X == pos.X && hitLeftPos.Y == pos.Y) {
					quitFlag = false;
				}
				else if (hitRightPos.X == pos.X && hitRightPos.Y == pos.Y) {
					quitFlag = false;
				}
			}
			return true;
		}
		else {
			return false;
		}

	}

	/****************************************
	Function:  Play()
	Parameter: None(void)
	Return:    None(void)
	Description:
	��ʼ��Ϸ
	*****************************************/
	void Play() {
		gameFlag = true;
		while (gameFlag) {
			checkChoice(); // �������

			// �鿴��ǰ�����Ƿ���Ҫ���±���
			if (posChoice.X != posChoiceOld.X || posChoice.Y != posChoiceOld.Y) {
				clearChoiceBackground(posChoiceOld);
				posChoiceOld = posChoice;
			}
			renderChoiceBackground(posChoice);

			// ��0,0�����õ�ǰѡ��λ�õ��ַ�����ע���β�пո����ף�����˼��ΪʲôҪ����ô��ո�
			MovePos(0, 0);
			char strPos[32] = "";
			sprintf_s(strPos, "��ǰѡ��(%u, %u)         ", posChoice.X, posChoice.Y);
			PutString(strPos);
			Update();

			// ��0,20�������ַ�������ʾʱ��
			MovePos(20, 0);
			timeNow = GetTickCount64();
			char strTime[32] = "";
			sprintf_s(strTime, "����ʱ�䣺%u         ", (isFirst ? 0 : (int)(timeNow - timeStart) / 1000));
			PutString(strTime);
			Update();

			// ��1,0�������ַ�������ʾʣ������
			MovePos(0, 1);
			char strBoom[32] = "";
			sprintf_s(strBoom, "ʣ��������%d         ", boomTotal - flagNum);
			PutString(strBoom);
			Update();

			// ��1,20�������ַ�������ʾ���߲���
			MovePos(20, 1);
			char strSlice[32] = "";
			sprintf_s(strSlice, "���߲�����%u         ", (isFirst ? 0 : alreadySlice));
			PutString(strSlice);
			Update();

			// ִ����Ӧ����
			switch (operation) {
			case 1:
				// �����ؿ�
				if (!isFirst) {
					alreadySlice++;
				}
				digBlock(posChoice.Y * mapWidth + posChoice.X);
				operation = 0;
				if (judgeWin()) {
					gameFlag = false;
				}
				break;
			case 2:
				// ���Ϊ����
				if (!isFirst) {
					alreadySlice++;
				}
				flagBlock(posChoice.Y * mapWidth + posChoice.X);
				operation = 0;
				break;
			}


			// �������ݲ������޸� ����ÿ֡������
			//updateMap(); // ���µ�ͼ����

			Update();    // ���²�������Ļ


			frame++;  // ��Ⱦ֡������
			clock_t elapsed = 25 - (clock() - tic); // �����һ֡��Ⱦʱ�䣬��������25�Ĳ�ֵ
			Sleep(elapsed > 0 ? elapsed : 0);	    // ����ֵ�����㣬�����߸ò�ֵ�ĺ���������ȷ��ÿ֡��Ⱦ������50֡
			tic = clock();						    // ������һ�μ�¼��ʱ��
		}
	}

	void DestroyGame() {
		for (int i = 0; i < mapWidth * mapWidth; i++) {		// ��ʼ����������
			mapCanvas[i] = 0;
			mapVisible[i] = 9;
		}
		isFirst = true;
		remainBlock = 0;
		flagNum = 0;
		alreadySlice = 1;
	}

}
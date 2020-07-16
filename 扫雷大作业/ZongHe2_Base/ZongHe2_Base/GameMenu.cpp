/****************************************
GameMenu.cpp
Author: Root
Date: 2019/12/09
Description:
�����˳���˵�����Ⱦ/����ģ�飬��GameMenu�����ռ��ֹͬ����Ⱦ
*****************************************/
#include "GameMenu.h"

namespace GameMenu {
	// �������������ռ�
	using namespace Graphic;
	using namespace Controller;

	size_t frame = 0;                  // ��������Ⱦ��֡�� 
	COORD pos = { 0 }, hitPos = { 0 }; // ������굱ǰ�������������������
	int choice = 0, oldChoice = 0;     // ����ѡ�еĲ˵���
	int subChoice = 0, subOldChoice = 0;	//�������ڴμ��˵��е�ѡ��˵���
	int diyChoice = 0, diyOldChoice = 0;	//���������Զ���˵��е�������
	int chatChoice = 0, chatOldChoice = 0;	//�������ڵ������ڵ�������
	int modeChoice = 0, modeOldChoice = 0;  //��������ģʽ���ڵ�������
	bool isChosen = false;             // �����Ƿ�����ѡ��
	bool subIsChosen = false;		   // �����ڴμ��˵��Ƿ�����ѡ��
	bool diyIsChosen = false;		   // �������Զ���˵��е��������Ƿ�ѡ��
	bool chatIsChosen = false;		   // ������chat���ڵ��������Ƿ�ѡ��
	bool modeIsChosen = false;		   // ������mode���ڵ��������Ƿ�ѡ��
	char key = '\0';                   // ���嵱ǰ���̵İ���
	clock_t tic = clock();             // ���������һ֡��Ⱦ��ʱ��
	// ��������¼ӵı�������������·���Ԥ�ñ���������
	int boomNum = 0;				   // ��������
	int mapWidth = 10;				   // �����ͼ���
	int mapHeight = 10;				   // �����ͼ�߶�
	int tempNum[3] = { 0,0,0 };	       // �����Զ�����ݴ���ֵ:0 ��������	1 ������	2 ����߶�
	int pin = 0, pin_ = 0;			   // ���ʱʹ�õĹ���ָ��
	int windowWidth = 44, windowHeight = 25; //�����ʼ���Ĵ��ڴ�С
	bool modeFinallyFlag = 0;		   // ��������ѡ���Ť��ģʽ���

	/****************************************
	Function:  outChat()
	Parameter: outString(char[])
	Return:    None(void)
	Description:
	���ڵ����Ի���Ŀ��壬�Ի������ּ�ΪoutSring�����ȱ���Ϊ18��
	*****************************************/
	void outChat(const char outString[]) {
		MovePos(windowWidth / 2 / 2 * 2 - 16, windowHeight / 2 - 6);
		PutString("                               ");
		MovePos(windowWidth / 2 / 2 * 2 - 16, windowHeight / 2 - 5);
		PutString("    �X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�j�T�T�T�T�T�T�[    ");
		MovePos(windowWidth / 2 / 2 * 2 - 16, windowHeight / 2 - 4);
		PutString("    �U              �U  ��  �U    ");
		MovePos(windowWidth / 2 / 2 * 2 - 16, windowHeight / 2 - 3);
		PutString("    �d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�m�T�T�T�T�T�T�g    ");
		MovePos(windowWidth / 2 / 2 * 2 - 16, windowHeight / 2 - 2);
		PutString("    �U                     �U    ");
		MovePos(windowWidth / 2 / 2 * 2 - 16, windowHeight / 2 - 1);
		PutString("    �^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a    ");
		MovePos(windowWidth / 2 / 2 * 2 - 16, windowHeight / 2 );
		PutString("                               ");
		MovePos(windowWidth / 2 / 2 * 2 - 7, windowHeight / 2 - 2);
		PutString(outString);
		Update();
	}

	/****************************************
	Function:  introMenu()
	Parameter: Nnoe(void)
	Return:    None(void)
	Description:
	���ڻ������ܲ˵�
	*****************************************/
	void introMenu() {
		ClearScreen();  // ����
		MovePos(16, 2); // �ƶ����굽 14,2
		PutString("ɨ����Ϸ���"); // ������������һ��string
		MovePos(6, 4);
		PutString("    ��ɨ�ס���һ������������С");
		MovePos(6, 5);
		PutString("��Ϸ����1992�귢�С���ϷĿ������");
		MovePos(6, 6);
		PutString("��̵�ʱ���ڸ��ݵ�����ӳ��ֵ���");
		MovePos(6, 7);
		PutString("���ҳ����з��׸��ӣ�ͬʱ����ȵ�");
		MovePos(6, 8);
		PutString("�ף��ȵ�һ���׼�ȫ�̽��䡣");
		MovePos(6, 10);
		PutString("    ������������ء�����������");

		Update();
	}

	/****************************************
	Function:  outNum()
	Parameter: outnum(int)
	Return:    None(void)
	Description:
	���Զ��廷���������ֵ����
	*****************************************/
	void outNum(int outnum) {
		if (outnum / 10 == 0) {

		}
		else {
			outNum(outnum / 10);
		}
		MovePos(pin_, pin);
		PutChar((char)((outnum % 10 )+ '0'));
		pin_++;
	}

	/****************************************
	Function:  renderMenu()
	Parameter: None(void)
	Return:    None(void)
	Description:
	�������Ҵ�ӡ�������ز˵�ѡ��
	*****************************************/
	void renderMenu() {
		ClearScreen();  // ����
		MovePos(14, 2); // �ƶ����굽 14,2
		PutString("ɨ�״���ս��"); // ������������һ��string
		MovePos(15, 4);
		PutString("��ʼս����");
		MovePos(15, 6);
		PutString("�鿴˵����");
		MovePos(15, 8);
		PutString("�˳���Ϸ��");
		Update();
	}

	/****************************************
	Function:  FormModeChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	ѡ��ɨ�׵�ģʽŤ�����
	*****************************************/
	void FormModeChoice() {
		ClearScreen();  // ����
		MovePos(12, 2);
		PutString(">>��ѡ��ģʽ<<");
		MovePos(16, 4);
		PutString("��  ��"); // ������������һ��string
		MovePos(16, 6);
		PutString("Ť  ��");
		Update();
	}

	/****************************************
	Function:  ModeChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	ѡ��ɨ�׵��ѶȻ����Զ����С������
	*****************************************/
	void ModeChoice() {
		ClearScreen();  // ����
		MovePos(12, 2);
		PutString(">>��ѡ���Ѷ�<<");
		MovePos(16, 4);
		PutString("��  ��"); // ������������һ��string
		MovePos(16, 6);
		PutString("��  ��");
		MovePos(16, 8);
		PutString("��  ��");
		MovePos(15, 10);
		PutString("�� �� ��");
		Update();
	}

	/****************************************
	Function:  DiyChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	ѡ��ɨ�׵��ѶȻ����Զ����С������
	*****************************************/
	void DiyChoice() {
		ClearScreen();		//����
		MovePos(11, 2);
		PutString(">>�Զ����Ѷȣ�<<");
		MovePos(8, 4);
		PutString("������");
		MovePos(8, 6);
		PutString("��ȣ�");
		MovePos(8, 8);
		PutString("�߶ȣ�");
		MovePos(17, 11);
		PutString("ȷ��");
		if (tempNum[0] != 0) {
			pin_ = 22;
			pin = 4;
			outNum(tempNum[0]);
		}
		if (tempNum[1] != 0) {
			pin_ = 22;
			pin = 6;
			outNum(tempNum[1]);
		}
		if (tempNum[2] != 0) {
			pin_ = 22;
			pin = 8;
			outNum(tempNum[2]);
		}
		Update();
	}

	/****************************************
	Function:  InitMenu()
	Parameter: None(void)
	Return:    None(void)
	Description:
	��ʼ���˵���ͬʱ��ʼ��ͼ�ο�ܺͿ��������
	*****************************************/
	void InitMenu() {
		InitGraphic(windowWidth, windowHeight); // ��ʼ���µĴ��ڴ�СΪ44,25
		InitController();    // ��ʼ��������
		renderMenu();        // ���Ʋ˵�
	}

	/****************************************
	Function:  randerChoiceBackground(int choice)
	Parameter: int choice
	Return:    None(void)
	Description:
	����ǰѡ�еĲ˵���(int choice)������Ⱦһ��������ʧ�İ�ɫ
	*****************************************/
	void randerChoiceBackground(int choice) {
		size_t y = 0;  // ���嵱ǰѡ�еĲ˵����ʵ��y����
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);  // ���屳����ɫ��ǰ����ɫֵ��ͨ������Ⱦ��֡���������㣨�˴�Ҳ���Ըĳ�ʹ��ʱ�䣩
		switch (choice) {
		case 0:
			y = 4;
			break;
		case 1:
			y = 6;
			break;
		case 2:
			y = 8;
			break;
		}
		for (size_t x = 10; x < 30; x++) {
			MovePos((SHORT)x, (SHORT)y); // �������Ƶ�Ŀ���
			ModColor(2, 255, 255, 255, alpha, alpha, alpha); // �޸ı���ɫ
		}
		Update();  // ���½���
	}

	/****************************************
	Function:  randerChatChoiceBackground(int choice)
	Parameter: int chatChoice
	Return:    None(void)
	Description:
	����ǰѡ�е�chat�˵���(int chatChoice)������Ⱦһ��������ʧ�İ�ɫ
	*****************************************/
	void randerChatChoiceBackground(int chatChoice) {
		int y = 0;  // ���嵱ǰѡ�еĲ˵����ʵ��y����
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);  // ���屳����ɫ��ǰ����ɫֵ��ͨ������Ⱦ��֡���������㣨�˴�Ҳ���Ըĳ�ʹ��ʱ�䣩
		for (int x = (windowWidth / 2 / 2 * 2 + 6); x < (windowWidth / 2 / 2 * 2 + 10); x++) {
			MovePos((SHORT)x, windowHeight / 2 - 4); // �������Ƶ�Ŀ���
			ModColor(2, 255, 255, 255, alpha, alpha, alpha); // �޸ı���ɫ
		}
		Update();  // ���½���
	}

	/****************************************
	Function:  randerDiyChoiceBackground(int choice)
	Parameter: int diyChoice
	Return:    None(void)
	Description:
	����ǰѡ�е�diy�˵���(int diyChoice)������Ⱦһ��������ʧ�İ�ɫ
	*****************************************/
	void randerDiyChoiceBackground(int diyChoice) {
		size_t y = 0;  // ���嵱ǰѡ�еĲ˵����ʵ��y����
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);  // ���屳����ɫ��ǰ����ɫֵ��ͨ������Ⱦ��֡���������㣨�˴�Ҳ���Ըĳ�ʹ��ʱ�䣩
		switch (diyChoice) {
		case 0:
			y = 4;
			break;
		case 1:
			y = 6;
			break;
		case 2:
			y = 8;
			break;
		case 3:
			y = 11;
			break;
		}
		for (size_t x = (diyChoice == 3 ? 10 : 21); x < (diyChoice == 3 ? 30 : 28); x++) {
			MovePos((SHORT)x, (SHORT)y); // �������Ƶ�Ŀ���
			ModColor(2, 255, 255, 255, alpha, alpha, alpha); // �޸ı���ɫ
		}
		Update();  // ���½���
	}

	/****************************************
	Function:  randerModeChoiceBackground(int choice)
	Parameter: int modeChoice
	Return:    None(void)
	Description:
	����ǰѡ�е�mode�˵���(int modeChoice)������Ⱦһ��������ʧ�İ�ɫ
	*****************************************/
	void randerModeChoiceBackground(int modeChoice) {
		size_t y = 0;  // ���嵱ǰѡ�еĲ˵����ʵ��y����
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);  // ���屳����ɫ��ǰ����ɫֵ��ͨ������Ⱦ��֡���������㣨�˴�Ҳ���Ըĳ�ʹ��ʱ�䣩
		switch (modeChoice) {
		case 0:
			y = 4;
			break;
		case 1:
			y = 6;
			break;
		}
		for (size_t x = 10 ; x < 30 ; x++) {
			MovePos((SHORT)x, (SHORT)y); // �������Ƶ�Ŀ���
			ModColor(2, 255, 255, 255, alpha, alpha, alpha); // �޸ı���ɫ
		}
		Update();  // ���½���
	}

	/****************************************
	Function:  randerSubChoiceBackground(int choice)
	Parameter: int subChoice
	Return:    None(void)
	Description:
	����ǰѡ�еĴμ��˵���(int subChoice)������Ⱦһ��������ʧ�İ�ɫ
	*****************************************/
	void randerSubChoiceBackground(int subChoice) {
		size_t y = 0;  // ���嵱ǰѡ�еĲ˵����ʵ��y����
		const BYTE alpha = 255 - (BYTE)((frame % 50) * 5);  // ���屳����ɫ��ǰ����ɫֵ��ͨ������Ⱦ��֡���������㣨�˴�Ҳ���Ըĳ�ʹ��ʱ�䣩
		switch (subChoice) {
		case 0:
			y = 4;
			break;
		case 1:
			y = 6;
			break;
		case 2:
			y = 8;
			break;
		case 3:
			y = 10;
		}
		for (size_t x = 10; x < 30; x++) {
			MovePos((SHORT)x, (SHORT)y); // �������Ƶ�Ŀ���
			ModColor(2, 255, 255, 255, alpha, alpha, alpha); // �޸ı���ɫ
		}
		Update();  // ���½���
	}

	/****************************************
	Function:  clearChoiceBackground(int choice)
	Parameter: int choice
	Return:    None(void)
	Description:
	���ϴ�ѡ�еĲ˵���(int choice)�����ָ�Ϊ��ɫ�����ݴ��º���һ������һ�£�����������
	*****************************************/
	void clearChoiceBackground(int choice) {
		size_t y = 0;
		switch (choice) {
		case 0:
			y = 4;
			break;
		case 1:
			y = 6;
			break;
		case 2:
			y = 8;
			break;
		}
		for (size_t x = 10; x < 30; x++) {
			MovePos((SHORT)x, (SHORT)y);
			ModColor(2, 255, 255, 255, 0, 0, 0);
		}
	}

	/****************************************
	Function:  clearChatChoiceBackground(int choice)
	Parameter: int diyChoice
	Return:    None(void)
	Description:
	���ϴ�ѡ�е�chat�˵���(int chatChoice)�����ָ�Ϊ��ɫ�����ݴ��º���һ������һ�£�����������
	*****************************************/
	void clearChatChoiceBackground(int chatChoice) {
		size_t y = 0;
		for (int x = (windowWidth / 2 / 2 * 2 + 6); x < (windowWidth / 2 / 2 * 2 + 10); x++) {
			MovePos((SHORT)x, windowHeight / 2 - 4);
			ModColor(2, 255, 255, 255, 0, 0, 0);
		}
	}

	/****************************************
	Function:  clearDiyChoiceBackground(int choice)
	Parameter: int diyChoice
	Return:    None(void)
	Description:
	���ϴ�ѡ�е�diy�˵���(int diyChoice)�����ָ�Ϊ��ɫ�����ݴ��º���һ������һ�£�����������
	*****************************************/
	void clearDiyChoiceBackground(int diyChoice) {
		size_t y = 0;
		switch (diyChoice) {
		case 0:
			y = 4;
			break;
		case 1:
			y = 6;
			break;
		case 2:
			y = 8;
			break;
		case 3:
			y = 11;
		}
		for (size_t x = (diyChoice == 3 ? 10 : 21); x < (diyChoice == 3 ? 30 : 28); x++) {
			MovePos((SHORT)x, (SHORT)y);
			ModColor(2, 255, 255, 255, 0, 0, 0);
		}
	}

	/****************************************
	Function:  clearSubChoiceBackground(int choice)
	Parameter: int subChoice
	Return:    None(void)
	Description:
	���ϴ�ѡ�еĴμ��˵���(int subChoice)�����ָ�Ϊ��ɫ�����ݴ��º���һ������һ�£�����������
	*****************************************/
	void clearSubChoiceBackground(int subChoice) {
		size_t y = 0;
		switch (subChoice) {
		case 0:
			y = 4;
			break;
		case 1:
			y = 6;
			break;
		case 2:
			y = 8;
			break;
		case 3:
			y = 10;
			break;
		}
		for (size_t x = 10; x < 30; x++) {
			MovePos((SHORT)x, (SHORT)y);
			ModColor(2, 255, 255, 255, 0, 0, 0);
		}
	}

	/****************************************
	Function:  clearModeChoiceBackground(int choice)
	Parameter: int modeChoice
	Return:    None(void)
	Description:
	���ϴ�ѡ�е�mode�˵���(int modeChoice)�����ָ�Ϊ��ɫ�����ݴ��º���һ������һ�£�����������
	*****************************************/
	void clearModeChoiceBackground(int modeChoice) {
		size_t y = 0;
		switch (modeChoice) {
		case 0:
			y = 4;
			break;
		case 1:
			y = 6;
			break;
		}
		for (size_t x = 10; x < 30; x++) {
			MovePos((SHORT)x, (SHORT)y);
			ModColor(2, 255, 255, 255, 0, 0, 0);
		}
	}

	/****************************************
	Function:  renderIntro()
	Parameter: None(void)
	Return:    None(void)
	Description:
	������Ϸ˵��
	*****************************************/
	void renderIntro() {
		introMenu();
		bool quitFlag = true;					// ����������˳�

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

			// ������Ҽ��ֱ��Ӧ����1��2
			if (hitLeftPos.X == pos.X && hitLeftPos.Y == pos.Y) {
				quitFlag = false;
			}
			else if (hitRightPos.X == pos.X && hitRightPos.Y == pos.Y) {
				quitFlag = false;
			}
		}
	}

	/****************************************
	Function:  checkSubChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	������/������룬������ѡ�еĴμ��˵���
	*****************************************/
	void checkSubChoice() {
		FlushInput();           // ˢ�����뻺����
		pos = GetCursorPos();   // ��ȡ�������
		key = GetKeyHit();		// ��ȡ��������
		hitPos = GetCursorHitPos();  // ��ȡ��굥������
		subIsChosen = false;		// ����ѡ��״̬

		// ������λ�ã�����ѡ����
		if (!(pos.Y != 4 || pos.X < 10 || pos.X > 30)) {
			subChoice = 0;
		}
		else if (!(pos.Y != 6 || pos.X < 10 || pos.X > 30)) {
			subChoice = 1;
		}
		else if (!(pos.Y != 8 || pos.X < 10 || pos.X > 30)) {
			subChoice = 2;
		}
		else if (!(pos.Y != 10 || pos.X < 10 || pos.X > 30)) {
			subChoice = 3;
		}

		// �����̰���������ѡ����
		switch (key) {
		case VK_UP:
			// �Ϸ����
			if (subChoice > 0)subChoice--;
			break;
		case VK_DOWN:
			// �·����
			if (subChoice < 3)subChoice++;
			break;
		case VK_RETURN:
			// �س�������ѡ��
			subIsChosen = true;
			break;
		}

		// ����������λ���Ƿ��ڲ˵����У�����ǣ�����ѡ��
		if (!(hitPos.Y != 4 || hitPos.X < 10 || hitPos.X > 30)) {
			subIsChosen = true;
		}
		else if (!(hitPos.Y != 6 || hitPos.X < 10 || hitPos.X > 30)) {
			subIsChosen = true;
		}
		else if (!(hitPos.Y != 8 || hitPos.X < 10 || hitPos.X > 30)) {
			subIsChosen = true;
		}
		else if (!(hitPos.Y != 10 || hitPos.X < 10 || hitPos.X > 30)) {
			subIsChosen = true;
		}
	}

	/****************************************
	Function:  checkModeChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	������/������룬������ѡ�е�mode�˵���
	*****************************************/
	void checkModeChoice() {
		FlushInput();           // ˢ�����뻺����
		pos = GetCursorPos();   // ��ȡ�������
		key = GetKeyHit();		// ��ȡ��������
		hitPos = GetCursorHitPos();  // ��ȡ��굥������
		subIsChosen = false;		// ����ѡ��״̬

		// ������λ�ã�����ѡ����
		if (!(pos.Y != 4 || pos.X < 10 || pos.X > 30)) {
			modeChoice = 0;
		}
		else if (!(pos.Y != 6 || pos.X < 10 || pos.X > 30)) {
			modeChoice = 1;
		}

		// �����̰���������ѡ����
		switch (key) {
		case VK_UP:
			// �Ϸ����
			if (modeChoice > 0)modeChoice--;
			break;
		case VK_DOWN:
			// �·����
			if (modeChoice < 1)modeChoice++;
			break;
		case VK_RETURN:
			// �س�������ѡ��
			modeIsChosen = true;
			break;
		}

		// ����������λ���Ƿ��ڲ˵����У�����ǣ�����ѡ��
		if (!(hitPos.Y != 4 || hitPos.X < 10 || hitPos.X > 30)) {
			modeIsChosen = true;
		}
		else if (!(hitPos.Y != 6 || hitPos.X < 10 || hitPos.X > 30)) {
			modeIsChosen = true;
		}
	}

	/****************************************
	Function:  checkChatChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	������/������룬������ѡ�еĴμ��˵���
	*****************************************/
	void checkChatChoice() {
		FlushInput();           // ˢ�����뻺����
		pos = GetCursorPos();   // ��ȡ�������
		key = GetKeyHit();		// ��ȡ��������
		hitPos = GetCursorHitPos();  // ��ȡ��굥������
		chatIsChosen = false;		// ����ѡ��״̬

		// ������λ�ã�����ѡ����
		if (!(pos.Y != windowHeight / 2 - 4 || pos.X < windowWidth / 2 + 7 || pos.X > windowWidth / 2 + 12)) {
			chatChoice = 0;
		}

		// �����̰���������ѡ����
		switch (key) {
		case VK_RETURN:
			// �س�������ѡ��
			chatIsChosen = true;
			break;
		}

		// ����������λ���Ƿ��ڲ˵����У�����ǣ�����ѡ��
		if (!(hitPos.Y != windowHeight / 2 - 4 || pos.X < windowWidth / 2 + 7 || pos.X > windowWidth / 2 + 12)) {
			chatIsChosen = true;
		}
	}

	/****************************************
	Function:  WaitChatChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	chat�˵���ѭ���������˳���һֱ��Ⱦ��ȥ��
	*****************************************/
	void WaitChatChoice() {
		bool runChatFlag = true;
		while (runChatFlag) {
			checkChatChoice();  // �������

			if (chatChoice != chatOldChoice) {
				// ���µ�ѡ�����ѡ��һ�£�����ѡ��ı������
				clearChatChoiceBackground(chatOldChoice);
				chatOldChoice = chatChoice;
			}
			randerChatChoiceBackground(chatChoice); // ����ѡ����ı���

			// ���ѡ�в˵���ִ�в���
			if (chatIsChosen) {
				switch (chatChoice) {
				case 0:
					runChatFlag = false;
					break;
				}
			}

			// ����ÿ֡������
			frame++;  // ��Ⱦ֡������
			clock_t elapsed = 25 - (clock() - tic); // �����һ֡��Ⱦʱ�䣬��������25�Ĳ�ֵ
			Sleep(elapsed > 0 ? elapsed : 0);       // ����ֵ�����㣬�����߸ò�ֵ�ĺ���������ȷ��ÿ֡��Ⱦ������50֡
			tic = clock();							// ������һ�μ�¼��ʱ��
		}
	}

	/****************************************
	Function:  checkDiyChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	������/������룬������ѡ�еĴμ��˵���
	*****************************************/
	void checkDiyChoice() {

		diyIsChosen = false;		// ����ѡ��״̬

		int tempPosition = 0;		//���������λ��

		for (;;) {
			FlushInput();           // ˢ�����뻺����
			pos = GetCursorPos();   // ��ȡ�������
			key = GetKeyHit();		// ��ȡ��������
			hitPos = GetCursorHitPos();  // ��ȡ��굥������

			// ������λ�ã�����ѡ����
			if (!(pos.Y != 4 || pos.X < 20 || pos.X > 29)) {
				diyChoice = 0;
			}
			else if (!(pos.Y != 6 || pos.X < 20 || pos.X > 29)) {
				diyChoice = 1;
			}
			else if (!(pos.Y != 8 || pos.X < 20 || pos.X > 29)) {
				diyChoice = 2;
			}
			else if (!(pos.Y != 11 || pos.X < 10 || pos.X > 30)) {
				diyChoice = 3;
			}

			// �����̰���������ѡ����
			switch (key) {
			case VK_UP:
				// �Ϸ����
				if (diyChoice > 0) {
					diyChoice--;
					if (diyChoice <= 2) {
						tempPosition = diyChoice;
					}
				}
				break;
			case VK_DOWN:
				// �·����
				if (diyChoice < 3) {
					diyChoice++;
					if (diyChoice <= 2) {
						tempPosition = diyChoice;
					}
				}
				break;
			case '0':
			case VK_NUMPAD0:
				// 0��
				if (diyChoice != 3) {
					if (tempNum[tempPosition] != 0) {
						tempNum[tempPosition] = tempNum[tempPosition] * 10;
					}
					DiyChoice();
				}
				break;
			case '1':
			case VK_NUMPAD1:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 1;
					DiyChoice();
				}
				break;
			case '2':
			case VK_NUMPAD2:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 2;
					DiyChoice();
				}
				break;
			case '3':
			case VK_NUMPAD3:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 3;
					DiyChoice();
				}
				break;
			case '4':
			case VK_NUMPAD4:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 4;
					DiyChoice();
				}
				break;
			case '5':
			case VK_NUMPAD5:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 5;
					DiyChoice();
				}
				break;
			case '6':
			case VK_NUMPAD6:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 6;
					DiyChoice();
				}
				break;
			case '7':
			case VK_NUMPAD7:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 7;
					DiyChoice();
				}
				break;
			case '8':
			case VK_NUMPAD8:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 8;
					DiyChoice();
				}
				break;
			case '9':
			case VK_NUMPAD9:
				if (diyChoice != 3) {
					tempNum[tempPosition] = tempNum[tempPosition] * 10 + 9;
					DiyChoice();
				}
				break;
			case VK_BACK:
				tempNum[tempPosition] /= 10;
				DiyChoice();
				break;
			case VK_RETURN:
				// �س�
				if (tempPosition < 2) {
					diyChoice++;
					tempPosition++;
				}
				else if (tempPosition >= 2) {
					diyIsChosen = true;
				}
				break;
			}

			// ����������λ���Ƿ��ڲ˵����У�����ǣ�����ѡ��
			if (!(hitPos.Y != 4 || hitPos.X < 21 || hitPos.X > 27)) {
				tempPosition = 0;
			}
			else if (!(hitPos.Y != 6 || hitPos.X < 21 || hitPos.X > 27)) {
				tempPosition = 1;
			}
			else if (!(hitPos.Y != 8 || hitPos.X < 21 || hitPos.X > 27)) {
				tempPosition = 2;
			}
			else if (!(hitPos.Y != 11 || hitPos.X < 15 || hitPos.X > 23)) {
				diyIsChosen = true;
			}

			if (diyIsChosen) {
				if (tempNum[1] <= 6) {
					outChat("�Զ�����Ӧ����6");
					WaitChatChoice();
					DiyChoice();
					diyIsChosen = false;
					continue;
				}
				if (tempNum[1] > 50) {
					outChat("�Զ����ӦС��51 ");
					WaitChatChoice();
					DiyChoice();
					diyIsChosen = false;
					continue;
				}
				if (tempNum[2] < 7) {
					outChat("�Զ���߶�Ӧ����6");
					WaitChatChoice();
					DiyChoice();
					diyIsChosen = false;
					continue;
				}
				if (tempNum[2] > 25) {
					outChat("�Զ��߶�ӦС��26 ");
					WaitChatChoice();
					DiyChoice();
					diyIsChosen = false;
					continue;
				}
				if (tempNum[0] < 9) {
					outChat("�Զ���������Ϊ10 ");
					WaitChatChoice();
					DiyChoice();
					diyIsChosen = false;
					continue;
				}
				if (tempNum[0] > tempNum[1] * tempNum[2] - 9) {
					outChat("������������࣡ ");
					WaitChatChoice();
					DiyChoice();
					diyIsChosen = false;
					continue;
				}
				boomNum = tempNum[0];
				mapWidth = tempNum[1];
				mapHeight = tempNum[2];
				break;
			}

			if (diyChoice != diyOldChoice) {
				// ���µ�ѡ�����ѡ��һ�£�����ѡ��ı������
				clearDiyChoiceBackground(diyOldChoice);
				diyOldChoice = diyChoice;
			}
			randerDiyChoiceBackground(diyChoice); // ����ѡ����ı���

			// ����ÿ֡������
			frame++;  // ��Ⱦ֡������
			clock_t elapsed = 25 - (clock() - tic); // �����һ֡��Ⱦʱ�䣬��������25�Ĳ�ֵ
			Sleep(elapsed > 0 ? elapsed : 0);       // ����ֵ�����㣬�����߸ò�ֵ�ĺ���������ȷ��ÿ֡��Ⱦ������50֡
			tic = clock();							// ������һ�μ�¼��ʱ��
		}

	}

	/****************************************
	Function:  checkChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	������/������룬������ѡ�еĲ˵���
	*****************************************/
	void checkChoice() {
		FlushInput();           // ˢ�����뻺����
		pos = GetCursorPos();   // ��ȡ�������
		key = GetKeyHit();		// ��ȡ��������
		hitPos = GetCursorHitPos();  // ��ȡ��굥������
		isChosen = false;		// ����ѡ��״̬

		// ������λ�ã�����ѡ����
		if (!(pos.Y != 4 || pos.X < 10 || pos.X > 30)) {
			choice = 0;
		}
		else if (!(pos.Y != 6 || pos.X < 10 || pos.X > 30)) {
			choice = 1;
		}
		else if (!(pos.Y != 8 || pos.X < 10 || pos.X > 30)) {
			choice = 2;
		}

		// �����̰���������ѡ����
		switch (key) {
		case VK_UP:
			// �Ϸ����
			if (choice > 0)choice--;
			break;
		case VK_DOWN:
			// �·����
			if (choice < 3)choice++;
			break;
		case VK_RETURN:
			// �س�������ѡ��
			isChosen = true;
			break;
		}

		// ����������λ���Ƿ��ڲ˵����У�����ǣ�����ѡ��
		if (!(hitPos.Y != 4 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 6 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
		else if (!(hitPos.Y != 8 || hitPos.X < 10 || hitPos.X > 30)) {
			isChosen = true;
		}
	}

	/****************************************
	Function:  WaitDiyChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	�ò˵������Զ����������趨��
	*****************************************/
	void WaitDiyChoice() {
		checkDiyChoice();  // �������
	}

	/****************************************
	Function:  WaitSubChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	�μ��˵���ѭ���������˳���һֱ��Ⱦ��ȥ������ɨ���Ѷȵ�ѡ����
	*****************************************/
	void WaitSubChoice() {
		bool runSubFlag = true;
		while (runSubFlag) {
			checkSubChoice();  // �������

			if (subChoice != subOldChoice) {
				// ���µ�ѡ�����ѡ��һ�£�����ѡ��ı������
				clearSubChoiceBackground(subOldChoice);
				subOldChoice = subChoice;
			}
			randerSubChoiceBackground(subChoice); // ����ѡ����ı���

			// ���ѡ�в˵���ִ�в���
			if (subIsChosen) {
				switch (subChoice) {
				case 0:
					// ��ʼ��Ϸ
					boomNum = 10;
					mapWidth = 10;
					mapHeight = 10;
					runSubFlag = false;
					break;
				case 1:
					boomNum = 40;
					mapWidth = 16;
					mapHeight = 16;
					runSubFlag = false;
					break;
				case 2:
					boomNum = 99;
					mapWidth = 30;
					mapHeight = 16;
					runSubFlag = false;
					break;
				case 3:
					DiyChoice();
					WaitDiyChoice();
					runSubFlag = false;
					break;
				}
			}

			// ����ÿ֡������
			frame++;  // ��Ⱦ֡������
			clock_t elapsed = 25 - (clock() - tic); // �����һ֡��Ⱦʱ�䣬��������25�Ĳ�ֵ
			Sleep(elapsed > 0 ? elapsed : 0);       // ����ֵ�����㣬�����߸ò�ֵ�ĺ���������ȷ��ÿ֡��Ⱦ������50֡
			tic = clock();							// ������һ�μ�¼��ʱ��
		}
	}

	/****************************************
	Function:  WaitModeChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	mode�˵���ѭ���������˳���һֱ��Ⱦ��ȥ������ɨ��ģʽ��ѡ����
	*****************************************/
	void WaitModeChoice() {
		bool runModeFlag = true;
		while (runModeFlag) {
			checkModeChoice();  // �������

			if (modeChoice != modeOldChoice) {
				// ���µ�ѡ�����ѡ��һ�£�����ѡ��ı������
				clearModeChoiceBackground(modeOldChoice);
				modeOldChoice = modeChoice;
			}
			randerModeChoiceBackground(modeChoice); // ����ѡ����ı���

			// ���ѡ�в˵���ִ�в���
			if (modeIsChosen) {
				switch (modeChoice) {
				case 0:
					// ��ʼ��Ϸ
					ModeChoice();
					WaitSubChoice();
					modeFinallyFlag = false;
					runModeFlag = false;
					break;
				case 1:
					ModeChoice();
					WaitSubChoice();
					modeFinallyFlag = true;
					runModeFlag = false;
					break;
				}
			}

			// ����ÿ֡������
			frame++;  // ��Ⱦ֡������
			clock_t elapsed = 25 - (clock() - tic); // �����һ֡��Ⱦʱ�䣬��������25�Ĳ�ֵ
			Sleep(elapsed > 0 ? elapsed : 0);       // ����ֵ�����㣬�����߸ò�ֵ�ĺ���������ȷ��ÿ֡��Ⱦ������50֡
			tic = clock();							// ������һ�μ�¼��ʱ��
		}
	}

	/****************************************
	Function:  ResetSize(int width,int height)
	Parameter: width(int), height(int)
	Return:    None(void)
	Description:
	�ú������ڸ��ݵ�ͼ�Ĵ�С���������д��ڵĴ�С����������ģ��
	*****************************************/
	void ResetSize(int width, int height) {
		windowWidth = 4 * width + 4;
		windowHeight = 2 * height + 5;
		InitMenu();
	}

	/****************************************
	Function:  ProResetSize(int width,int height)
	Parameter: width(int), height(int)
	Return:    None(void)
	Description:
	�ú������ڸ��ݵ�ͼ�Ĵ�С���������д��ڵĴ�С������Ť���ռ�ģ��
	*****************************************/
	void ProResetSize(int width, int height) {
		windowWidth = (3 * width + 2) / 4 * 4 + 4;
		windowWidth = (windowWidth < 40 ? 40 : windowWidth);
		windowHeight = 2 * (height) + 6;
		InitMenu();
	}

	/****************************************
	Function:  WaitChoice()
	Parameter: None(void)
	Return:    None(void)
	Description:
	��ѭ���������˳���һֱ��Ⱦ��ȥ
	*****************************************/
	void WaitChoice() {
		bool runFlag = true;
		while (runFlag) {
			checkChoice();  // �������

			if (choice != oldChoice) {
				// ���µ�ѡ�����ѡ��һ�£�����ѡ��ı������
				clearChoiceBackground(oldChoice);
				oldChoice = choice;
			}
			randerChoiceBackground(choice); // ����ѡ����ı���

			// ���ѡ�в˵���ִ�в���
			if (isChosen) {
				switch (choice) {
				case 0:
					// ��ʼ��Ϸ
					FormModeChoice();
					WaitModeChoice();
					if (modeFinallyFlag) {
						ProResetSize(mapWidth, mapHeight);
						GameEnginePro::InitGame(mapWidth, mapHeight, boomNum);
						GameEnginePro::Play();
						GameEnginePro::DestroyGame();
					}
					else {
						ResetSize(mapWidth, mapHeight);
						GameEngine::InitGame(mapWidth, mapHeight, boomNum);
						GameEngine::Play();
						GameEngine::DestroyGame();
					}
					boomNum = 0;
					mapWidth = 0;
					mapHeight = 0;
					windowHeight = 25;
					windowWidth = 44;
					InitMenu();
					renderMenu();
					tempNum[0] = 0;
					tempNum[1] = 0;
					tempNum[2] = 0;
					modeIsChosen = false;
					break;
				case 1:
					// ����˵��
					renderIntro();
					renderMenu();
					break;
				case 2:
					// ��Flag�÷����˳�ѭ��
					runFlag = false;
					break;
				}
			}

			// ����ÿ֡������
			frame++;  // ��Ⱦ֡������
			clock_t elapsed = 25 - (clock() - tic); // �����һ֡��Ⱦʱ�䣬��������25�Ĳ�ֵ
			Sleep(elapsed > 0 ? elapsed : 0);       // ����ֵ�����㣬�����߸ò�ֵ�ĺ���������ȷ��ÿ֡��Ⱦ������50֡
			tic = clock();							// ������һ�μ�¼��ʱ��
		}
	}
}
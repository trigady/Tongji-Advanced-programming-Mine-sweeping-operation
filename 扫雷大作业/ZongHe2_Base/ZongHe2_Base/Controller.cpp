/****************************************
Controller.cpp
Author: Root
Date: 2019/12/09
Description:
�����˿�������ܣ���Controller�����ռ��ֹͬ����Ⱦ
��*��ע�⣺����������ȫ�����ڲ�������ʲô���������򲻽����޸�����ļ���*��
*****************************************/
#include "Controller.h"

namespace Controller {
	COORD pos = { 0 };                   // �������
	HANDLE hStdIn = NULL;				 // ��׼������
	INPUT_RECORD inRecArr[256] = { 0 };	 // ���뻺��������
	INPUT_RECORD inRec = { 0 };			 // ������Ҫ��ȡ������
	DWORD dRead = 0;					 // ��ȡ�������뻺��������
	char key = '0';						 // ���̰���
	const DWORD MOVE = 0, LEFT_CLICK = 1, RIGHT_CLICK = 2, _DOUBLE_CLICK = 3; // һЩ����

	/****************************************
	Function:  updateInput(DWORD type, DWORD hitMode = MOVE)
	Parameter: DWORD type, hitMode
	Return:    bool
	Description:
	��inRec��������Ϊ��Ҫ��ȡ����������type(�������)������ҵ���Ҫ�����룬�򷵻�true������inRec���������򷵻�false
	hitMode��ָ�����ĵ��ģʽ�����������/�Ҽ�������˫��
	*****************************************/
	bool updateInput(DWORD type, DWORD hitMode = MOVE) {
		// �����뻺���������һ�����ݿ�ʼѭ������
		for (int i = (int)dRead; i > 0; i--) {
			// ���������ݵ����ͷ�������
			if (inRecArr[i - 1].EventType == type) {
				// ���hitMode �����MOVE������switch
				switch (hitMode) {
				case LEFT_CLICK:
					// �������
					if (inRecArr[i - 1].Event.MouseEvent.dwButtonState != 1) {
						continue;
					}
					break;
				case RIGHT_CLICK:
					// �Ҽ�����
					if (inRecArr[i - 1].Event.MouseEvent.dwButtonState != RIGHTMOST_BUTTON_PRESSED) {
						continue;
					}
					break;
				case _DOUBLE_CLICK:
					// ˫��
					if (inRecArr[i - 1].Event.MouseEvent.dwEventFlags != DOUBLE_CLICK) {
						continue;
					}
					break;
				}
				// ����inRec������true��ʾ�ҵ���Ҫ������
				inRec = inRecArr[i - 1];
				return true;
			}
		}
		return false;
	}

	/****************************************
	Function:  FlushInput()
	Parameter: None(void)
	Return:    None(void)
	Description:
	ˢ�����뻺������ȡ��������ȫ������
	*****************************************/
	void FlushInput() {
		if (WaitForSingleObject(hStdIn, 0) == WAIT_OBJECT_0) {
			// ����������������ݣ���������ݣ����256��
			ReadConsoleInputA(hStdIn, &inRecArr[0], 256, &dRead);
		}
		else {
			// ����Read��Ϊ0
			dRead = 0;
		}
	}

	/****************************************
	Function:  GetKeyHit()
	Parameter: None(void)
	Return:    char
	Description:
	��ȡ�������룬���û�м��̻��У��򷵻�'\0'
	*****************************************/
	char GetKeyHit() {
		if (updateInput(KEY_EVENT)) {
			if (inRec.Event.KeyEvent.bKeyDown) {
				key = (char)inRec.Event.KeyEvent.wVirtualKeyCode;
				return key;
			}
		}
		return '\0';
	}

	/****************************************
	Function:  GetCursorPos()
	Parameter: None(void)
	Return:    COORD
	Description:
	��ȡ������룬��һֱ������һ�θ��µ�����
	*****************************************/
	COORD GetCursorPos() {
		if (updateInput(MOUSE_EVENT)) {
			pos = inRec.Event.MouseEvent.dwMousePosition;
		}
		return pos;
	}

	/****************************************
	Function:  GetCursorHitPos()
	Parameter: DWORD type
	Return:    COORD
	Description:
	��ȡ��������룬type��ָ�����������(1)�Ҽ�����(2)˫��(3)����û�е������һֱ����0,0
	*****************************************/
	COORD GetCursorHitPos(DWORD type) {
		if (updateInput(MOUSE_EVENT, type)) {
			return inRec.Event.MouseEvent.dwMousePosition;
		}
		return { 0,0 };
	}

	/****************************************
	Function:  InitController()
	Parameter: None(void)
	Return:    None(void)
	Description:
	��ʼ��������
	*****************************************/
	void InitController() {
		hStdIn = GetStdHandle(STD_INPUT_HANDLE); // ��ȡ��׼������
		SetConsoleMode(hStdIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);  // ������̨����Ϊ�����������
	}
}